#include "decompress.h"

using namespace std;

Decompress::Decompress(QString filename)
{
    this->filename = filename;
}

void Decompress::decompress(QString filename)
{
    Decompress *d = new Decompress(filename);
    d->decompress();
    delete d;
}

void Decompress::decompress()
{
    qDebug() << filename;

    this->dataIn = new char[READ_BUFFER_LENGHT];
    this->in = new ifstream(filename.toStdString(), ifstream::binary);

    this->buffer = new char[WRITE_BUFFER_LENGHT];
    this->indexBuffer = 0;
    this->out = new ofstream(Util::getDecompressedFilename(this->filename), std::ios::out);

    char ch;
    in->read(&ch, 1);
    bitsDespisedContent = ch-'0';

    int len = Util::nextUInt(in);
    in->read(dataIn, len);
    tags = HuffElement::buildTree(dataIn, true);

    len = Util::nextUInt(in);
    in->read(dataIn, len);
    attrs = HuffElement::buildTree(dataIn, false);

    len = Util::nextUInt(in);
    in->read(dataIn, len);
    numbers = HuffmanTree::buildTree(dataIn);

    len = Util::nextUInt(in);
    in->read(dataIn, len);
    letters = HuffmanTree::buildTree(dataIn);

    this->in->read(this->dataIn, READ_BUFFER_LENGHT);
    this->numberOfBytesRead = this->in->gcount();
    this->indexIn = 0;
    this->mask = 128;
    this->byte = this->dataIn[0];

    this->readTag(tags);

    while(true){
        //qDebug() << "separador";
        if(this->readSeparator()){
            //qDebug() << "tag";
            if(this->readTag(tags)) break;
            //qDebug() << "/tag";
        } else {
            //qDebug() << "conteudo";
            if(this->readCont(stack.top()->getType()==Element::NUMBER)) break;
            //qDebug() << "/conteudo";
        }
        //qDebug() << "/separador";

    }
    while(!stack.empty()){
        this->closeTag();
    }

    this->out->flush();
    this->out->close();
    this->in->close();
    qDebug() << "fim";

    /*QString s = "";
    while(!in->eof()){
        in->read(dataIn, BUFFER_LENGHT);
        int r = in->gcount();
        qDebug() << r;
        for(int k=0;k<r;k++){
            unsigned char ch = (unsigned char) dataIn[k];
            this->byte = ch;
            for(unsigned char m=128;m>0;m>>=1){
                s += (this->readBit() ? "1" : "0");
            }
        }
    }
    tags->print();
    qDebug() <<s.size();
    qDebug() << s;*/

}

bool Decompress::readTag(HuffElement *tags)
{
    while(!tags->isLeaf()){
        tags = (HuffElement*) (readBit() ? tags->getRight() : tags->getLeft());
    }

    incompleteTag = false;
    for(int k=0,t=stack.size();k<t;k++) *out << "\t";
    if(!stack.empty()) stack.top()->setContent(true);
    Element *e = tags->getElement();
    stack.push(e);
    switch (e->getToken()) {
        case Element::COMMENT: *out << "<!-- "; break;
        case Element::DTD: break;
        case Element::START_DOCUMENT: *out << "<" << *(e->getName()); break;
        default:
            *out << "<" << *(e->getName());
            this->incompleteTag = true;
            break;
    }
    int numberOfAttributes = this->readInt();
    for(int k=0;k<numberOfAttributes;k++){
        this->readAttr(this->attrs);
    }
    this->lastWasContent = false;
    unsigned char m = 255;
    m<<=(bitsDespisedContent-1);
    return (in->eof() && this->indexIn==(numberOfBytesRead-1) && (m & this->mask));
}

void Decompress::readAttr(HuffElement *attrs)
{
    while(!attrs->isLeaf()){
        attrs = (HuffElement*)((readBit()) ? attrs->getRight() : attrs->getLeft());
    }
    Element *e = attrs->getElement();
    e->setToken(Element::ATTRIBUTE);
    stack.push(e);
    *out << " " << *(e->getName()) << "=\"";
    this->readCont(e->getType()==Element::NUMBER);
    *out << "\"";
    stack.pop();
}

bool Decompress::readSeparator()
{
    if(this->readBit()){
        int n = this->readInt();
        for(int k=0;k<n;k++) this->closeTag();
    } else {
        if(readBit()){
            if(incompleteTag){
                incompleteTag = false;
                *out << ">\n";
            }
            stack.top()->setContent(true);
        } else {
            if(!lastWasContent) this->closeTag();
        }
    }
    return readBit();
}



bool Decompress::readCont(bool isNumber)
{
    int token = stack.top()->getToken();
    HuffmanTree* tree = (isNumber) ? this->numbers : this->letters;
    this->indexBuffer = 0;
    if(token!=Element::ATTRIBUTE && token!=Element::COMMENT){
        int t = (token==Element::DTD) ? stack.size()-1 : stack.size();
        for(int k=0;k<t;k++)
            this->buffer[this->indexBuffer++] = '\t';
    }

    HuffmanTree* nodo = tree;
    while(true){
        if(indexBuffer==WRITE_BUFFER_LENGHT){
            out->write(buffer, indexBuffer);
            indexBuffer = 0;
        }
        while(!nodo->isLeaf()){
            nodo = (readBit()) ? nodo->getRight() : nodo->getLeft();
        }
        if(nodo->getValue()==0){
            int token = stack.top()->getToken();
            if(token==Element::NORMAL || token==Element::DTD) buffer[indexBuffer++] = '\n';
            out->write(buffer, indexBuffer);
            break;
        } else {
            buffer[indexBuffer++] = (char) nodo->getValue();
            nodo = tree;
        }
    }

    unsigned char m = 255;
    m<<=(bitsDespisedContent-1);
    this->lastWasContent = true;
    return (in->eof() && this->indexIn==(numberOfBytesRead-1) && (m & this->mask));
}


int Decompress::readInt()
{
    int res = 0;
    int len = 1;
    if(readBit()){
        if(readBit()) len = 4;
        else len = 2;
    }
    int aux = (len==1) ? 64 : (len==2) ? 8192 : 536870912;
    while(aux>0){
        if(readBit()) { res += aux;}
        aux/=2;
    }
    return res;
}

