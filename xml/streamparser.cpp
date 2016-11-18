#include "streamparser.h"

using namespace std;

StreamParser::StreamParser(QString filename)
{
    this->xmlFile = new QFile(filename);
    this->xmlFile->open(QIODevice::ReadOnly);
    this->xml.setDevice(xmlFile);
    this->end = false;
}

StreamParser::~StreamParser()
{

    delete this->xmlFile;
}

void StreamParser::countFrequence(int *contNumbers, int* conLetters, Patricia<Element*>* patTags, Patricia<Element*>* patAtr)
{
    for(int k=0;k<256;k++){
        conLetters[k] = contNumbers[k] = 0;
    }
    while(!this->end){
        switch (xml.tokenType()) {
            case QXmlStreamReader::EndDocument:
                this->end = true;
                break;
            case QXmlStreamReader::Invalid:
                invalid();
                exit(0);
            case QXmlStreamReader::StartElement:{
                this->contTag(xml.name(), xml.prefix(), patTags, false);
                if(!xml.namespaceDeclarations().isEmpty()){
                    foreach(QXmlStreamNamespaceDeclaration nd, xml.namespaceDeclarations()){
                        QString name = "xmlns";
                        if(nd.prefix().isEmpty()){
                            this->contAtt(&name, nd.prefix(), nd.namespaceUri(),
                                          contNumbers, conLetters, patAtr);
                        } else {
                            this->contAtt(nd.prefix(), &name, nd.namespaceUri(),
                                          contNumbers, conLetters, patAtr);
                        }

                    }
                }
                if(!xml.attributes().isEmpty()){
                    foreach(QXmlStreamAttribute at, xml.attributes()){
                        this->contAtt(at.name(), at.prefix(), at.value(), contNumbers, conLetters, patAtr);
                    }
                }
                break;
            }
            case QXmlStreamReader::EndElement:
                types.pop();
                break;
            case QXmlStreamReader::StartDocument: {
                QString xmlStr = "?xml";
                this->contTag(&xmlStr, nullptr, patTags, false);
                if(!xml.documentVersion().isEmpty()){
                    QString version = "version";
                    this->contAtt(&version, nullptr, xml.documentVersion(),
                                  contNumbers, conLetters, patAtr);
                }
                if(!xml.documentEncoding().isEmpty()){
                    QString encoding = "encoding";
                    this->contAtt(&encoding, nullptr, xml.documentEncoding(),
                                                  contNumbers, conLetters, patAtr);
                }
                if(xml.isStandaloneDocument()){
                    QString stan = "standalone";
                    QString confirm = "yes";
                    this->contAtt(&stan, nullptr, &confirm, contNumbers, conLetters, patAtr);
                }
                types.pop();
                break;
            }
            case QXmlStreamReader::Comment:{
                QString comment = "--";
                this->contTag(&comment, nullptr, patTags, false);
                this->contChar(xml.text(), conLetters);
                types.pop();
                break;
            }
            case QXmlStreamReader::DTD:{
                QString dtd = "!DTD";
                this->contTag(&dtd, nullptr, patTags, false);
                this->contChar(xml.text(), conLetters);
                types.pop();
                break;
            }
            case QXmlStreamReader::Characters:{
                int *cont = (types.top()==Element::NUMBER) ? contNumbers : conLetters;
                this->contChar(xml.text(), cont);
                break;
            }
            default: break;
        }
        xml.readNext();
    }
}

void StreamParser::compress(ofstream* out, Patricia<Element *> *tags, Patricia<Element *> *attr, std::unordered_map<unsigned char, HuffBits> *num, std::unordered_map<unsigned char, HuffBits> *let)
{
    this->buffer = new char[BUFFER_LENGHT];
    this->lenghtBuffer = 0;
    this->contSeparator = 0;
    this->mask = 128;
    this->byte = 0;
    this->out = out;
    while(!this->end){
        switch (xml.tokenType()) {
            case QXmlStreamReader::EndDocument:
                this->end = true;
                break;
            case QXmlStreamReader::Invalid:
                invalid();
                exit(0);
            case QXmlStreamReader::StartElement:{
                this->writeSeparator(true);
                this->contSeparator = -1;
                this->writeTag(xml.name(),xml.prefix(),tags);
                int u = xml.namespaceDeclarations().size();
                int v = xml.attributes().size();
                this->writeNumberOfAttributes(u+v);
                if(u>0){
                    foreach(QXmlStreamNamespaceDeclaration nd, xml.namespaceDeclarations()){
                        QString name = "xmlns";
                        if(nd.prefix().isEmpty()){
                            this->writeAttr(&name, nd.prefix(), nd.namespaceUri(),num, let, attr);
                        } else {
                            this->writeAttr(nd.prefix(), &name, nd.namespaceUri(),num, let, attr);
                        }
                    }
                }
                if(v>0)
                    foreach(QXmlStreamAttribute at, xml.attributes())
                        this->writeAttr(at.name(), at.prefix(), at.value(), num, let, attr);
                break;
            }
            case QXmlStreamReader::EndElement:
                if(contSeparator<0) contSeparator = 0;
                contSeparator++;
                types.pop();
                break;
            case QXmlStreamReader::StartDocument: {
                QString xmlStr = "?xml";
                this->writeTag(&xmlStr, nullptr, tags);
                int numberOfAttributes = int(!xml.documentVersion().isEmpty())+
                        int(!xml.documentEncoding().isEmpty())+int(xml.isStandaloneDocument());
                this->writeNumberOfAttributes(numberOfAttributes);

                if(!xml.documentVersion().isEmpty()){
                    QString version = "version";
                    this->writeAttr(&version, nullptr, xml.documentVersion(), num, let, attr);
                }
                if(!xml.documentEncoding().isEmpty()){
                    QString encoding = "encoding";
                    this->writeAttr(&encoding, nullptr, xml.documentEncoding(), num, let, attr);
                }
                if(xml.isStandaloneDocument()){
                    QString stan = "standalone";
                    QString confirm = "yes";
                    this->writeAttr(&stan, nullptr, &confirm, num, let, attr);
                }
                this->contSeparator = 0;
                types.pop();
                break;
            }
            case QXmlStreamReader::Comment:{
                this->writeSeparator(true);
                QString comment = "--";
                this->writeTag(&comment, nullptr, tags);
                this->writeNumberOfAttributes(0);
                this->contSeparator = -1;
                this->writeSeparator(false);
                this->writeChar(Util::trim(xml.text()), let, false);
                contSeparator++;
                types.pop();
                break;
            }
            case QXmlStreamReader::DTD:{
                this->writeSeparator(true);
                QString dtd = "!DTD";
                this->writeTag(&dtd, nullptr, tags);
                this->writeNumberOfAttributes(0);
                this->contSeparator = -1;
                this->writeSeparator(false);
                this->writeChar(Util::trim(xml.text()), let, false);
                contSeparator++;
                types.pop();
                break;
            }
            case QXmlStreamReader::Characters:{
                string s = Util::trim(xml.text());
                if(!s.empty()){
                    unordered_map<unsigned char,HuffBits> *map = (types.top()==Element::NUMBER) ? num : let;
                    this->writeSeparator(false);
                    this->writeChar(s, map, false);
                }
                break;
            }
            default: break;
        }
        xml.readNext();
    }
    int bitsDespisedContent = 0;
    if(mask>0 && mask<128){
       this->buffer[this->lenghtBuffer++] = this->byte;
       while(mask>0 && mask<128) {
           bitsDespisedContent++;
           mask>>=1;
       }
    }
    if(this->lenghtBuffer>0){
        /*string s = "";
        for(int k=0;k<lenghtBuffer;k++){
            unsigned char ch = (unsigned char) buffer[k];
            for(unsigned char m=128;m>0;m>>=1){
                s += (ch & m) ? "1" : "0";
            }
        }
        cout << s << endl;*/
        this->out->write(this->buffer, this->lenghtBuffer);
        this->lenghtBuffer = 0;
    }
    this->out->seekp(0);

    *(this->out) << bitsDespisedContent;
}


void StreamParser::reset()
{
    this->end = false;
    this->xmlFile->close();
    this->xmlFile->open(QIODevice::ReadOnly);
    this->xml.setDevice(this->xmlFile);
}

void StreamParser::invalid()
{
    qDebug() << xml.errorString();
    qDebug() << xml.lineNumber();
    qDebug() << xml.columnNumber();
    qDebug() << xml.characterOffset();
    qDebug() << "Token inválido!";
}

void StreamParser::contChar(const QStringRef content, int *cont)
{
    string s = Util::trim(content);
    int tam = s.length();
    if(tam>0){
        const char *data = s.c_str();
        for(int k=0;k<tam;k++){
            unsigned char c = (unsigned char) data[k];
            cont[c]++;
        }
        cont[0]++;
    }
}

int StreamParser::contTag(const QStringRef name, const QStringRef prefix, Patricia<Element*> *pat, bool isAttr)
{
    int res = Element::TEXT;
    string tag = name.toString().toStdString();
    if(!prefix.isEmpty()) tag = prefix.toString().toStdString()+":"+tag;
    Element** e = pat->get(tag);
    if(e==nullptr) pat->insert(tag, (isAttr ? new Element(tag, 1, Element::ATTRIBUTE) : new Element(tag, 1)));
    else {
        (*e)->increaseAmount();
        res = (*e)->getType();
    }

    types.push(res);
    return res;
}

int StreamParser::contAtt(const QStringRef name, const QStringRef prefix, const QStringRef content, int *contNumbers, int *conLetters, Patricia<Element *> *pat)
{
    int type = this->contTag(name, prefix, pat, true);
    int *cont = (type==Element::NUMBER) ? contNumbers : conLetters;
    this->contChar(content, cont);
    types.pop();
    return type;
}

void StreamParser::writeSeparator(bool isTag)
{
    if(contSeparator>0){
        int len;
        this->writeBit();
        char* aux = Util::convertUIntToChar(this->contSeparator, len);
        for(int k=0;k<len;k++) {
            char ch = (unsigned char) aux[k];
            for(unsigned char m=128;m>0;m>>=1){
                this->writeBit(ch & m);
            }
        }
        this->contSeparator = 0;
    } else {
        this->increaseMask();
        this->writeBit(this->contSeparator < 0);
    }
    this->writeBit(isTag);
}

int StreamParser::writeTag(QStringRef name, QStringRef prefix, Patricia<Element *> *pat)
{
    string tag = name.toString().toStdString();
    if(!prefix.isEmpty()) tag = prefix.toString().toStdString()+":"+tag;

    Element** e = pat->get(tag);
    Bits* b = (*e)->getBits();

    int i = 0;
    for(int k=0,t=ceil(b->length/8.0);k<t;k++){
        unsigned char ch = (unsigned char) b->value[k];
        for(unsigned char m=128;m>0 && i<b->length;i++,m>>=1)
            this->writeBit(ch & m);
    }
    types.push((*e)->getType());
    return (*e)->getType();
}

int StreamParser::writeAttr(const QStringRef name, const QStringRef prefix, const QStringRef content, std::unordered_map<unsigned char, HuffBits> *num, std::unordered_map<unsigned char, HuffBits> *let, Patricia<Element *> *pat)
{
    int type = this->writeTag(name, prefix, pat);
    unordered_map<unsigned char,HuffBits>* map = (type==Element::NUMBER) ? num : let;
    this->writeChar(Util::trim(content), map, true);
    types.pop();
    return type;
}

void StreamParser::writeChar(std::string content, std::unordered_map<unsigned char, HuffBits> *map, bool isAtrr)
{
    const char *data = content.c_str();
    for(int j=0,tam=content.length()+1;j<tam;j++){
        unsigned char c = (unsigned char) data[j];
        HuffBits hb = map->at(c);

        /*if(content=="1.0"){

            QString a = "";
            int i = 0;
            for(int k=0,t=ceil(hb.length/8.0);k<t;k++){
                unsigned char ch = (unsigned char) hb.value[k];
                for(unsigned char m=128;m>0 && i<hb.length;i++,m>>=1)
                    a += (ch & m) ? "1" : "0";
            }
            qDebug() << QChar(c) << int(c) << " " << a;

        }*/



        int i = 0;
        for(int k=0,t=ceil(hb.length/8.0);k<t;k++){
            unsigned char ch = (unsigned char) hb.value[k];
            for(unsigned char m=128;m>0 && i<hb.length;i++,m>>=1)
                this->writeBit(ch & m);
        }
    }
    if(!isAtrr) this->contSeparator = 0;
}

