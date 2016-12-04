#include "compress.h"

using namespace std;

Compress::Compress(QString &filename)
{
    this->filename = filename;
    this->parser = new StreamParser(filename);
}

Compress::~Compress()
{
    delete this->parser;
}


void Compress::compress(QString filename)
{
    Compress *c = new Compress(filename);
    c->compress();
    delete c;
}

void Compress::compress()
{
    int* contLet = new int[256];
    int* contNum = new int[256];
    Patricia<Element*> *contTag = Util::tags();
    Patricia<Element*> *contAttr = Util::attributes();
    this->parser->countFrequence(contNum, contLet, contTag, contAttr);

    HuffElement *huffTags = this->createHuffElement(contTag);
    HuffElement *huffAttr = this->createHuffElement(contAttr);
    HuffmanTree *treeNum = this->createHuffTree(contNum);
    HuffmanTree *treeLet = this->createHuffTree(contLet);

    unordered_map<unsigned char,HuffBits>* tableNum = treeNum->toDictionary();
    unordered_map<unsigned char,HuffBits>* tableLet = treeLet->toDictionary();
    Patricia<Element*>* tableTags = huffTags->toDictionary();
    Patricia<Element*>* tableAttr = huffAttr->toDictionary();

   /* for(unordered_map<unsigned char,HuffBits>::iterator it=tableLet->begin();
        it!=tableLet->end();it++){
        unsigned ch = *(&it->first);
        HuffBits h = *(&it->second);

        unsigned char m = 128;
        string s = "";
        for(int k=0;k<h.length;k++,m>>=1){
            int index = k/8;
            if(m==0) m=128;
            unsigned char c = (unsigned char) h.value[index];
            s += (c & m) ? "1" : "0";
        }

        string o = treeLet->find(ch);

        if(o!=s){
            cout << int(ch) << " " << ch << " " << o << " " << s << endl;
        }

        if(ch==0){
            cout << int(ch) << " " << ch << " " << o << " " << s << endl;
        }
    }
*/


    this->out = new ofstream(Util::getOutputFilename(this->filename), ofstream::binary);
    *(this->out) << " ";
    this->writeTrees(huffTags, huffAttr, treeNum, treeLet);
    this->write(tableTags, tableAttr, tableNum, tableLet);
    this->out->close();
    delete out;
    qDebug() << "compressed " << filename;
}


HuffElement *Compress::createHuffElement(Patricia<Element *> *patricia)
{
    PriorityQueue *pq = new PriorityQueue();
    for(Patricia<Element*>::iterator it=patricia->begin();
        it!=patricia->end();it++){
        Element *e = *(+it);
        if(e->getAmount()>0){
            HuffElement* he = new HuffElement(e, e->getAmount());
            pq->insert(he);
        }
    }
    HuffElement *one = (HuffElement*) pq->removeMin();
    while(!pq->isEmpty()){
        HuffElement *two = (HuffElement*) pq->removeMin();
        HuffElement *both = new HuffElement(nullptr, one->getFrequence()+two->getFrequence());
        both->setLeft(one);
        both->setRight(two);
        pq->insert(both);
        one = (HuffElement*) pq->removeMin();
    }
    return one;
}

HuffmanTree *Compress::createHuffTree(int *cont)
{
    PriorityQueue *pq = new PriorityQueue();
    for(int k=0;k<256;k++){
        if(cont[k]>0){
            HuffmanTree* value = new HuffmanTree(k, cont[k]);
            pq->insert(value);
        }
    }
    if(pq->isEmpty()) return new HuffmanTree();

    HuffmanTree *one = (HuffmanTree*) pq->removeMin();
    while(!pq->isEmpty()){
        HuffmanTree *two = (HuffmanTree*) pq->removeMin();
        HuffmanTree *both = new HuffmanTree(-1, one->getFrequence()+two->getFrequence());
        both->setLeft(one);
        both->setRight(two);
        pq->insert(both);
        one = (HuffmanTree*) pq->removeMin();
    }
    return one;
}

void Compress::writeTrees(HuffElement *tags, HuffElement *attr, HuffmanTree *num, HuffmanTree *let)
{
    char* aux;
    int tAux, len;
    unsigned char* tree;

    tree = tags->toBinary(len);
    aux = Util::convertUIntToChar(len, tAux);
    this->out->write(aux,tAux);
    this->out->write(reinterpret_cast<char*>(tree), len);
    delete[] tree;

    tree = attr->toBinary(len);
    aux = Util::convertUIntToChar(len, tAux);
    this->out->write(aux,tAux);
    this->out->write(reinterpret_cast<char*>(tree), len);
    delete[] tree;

    tree = num->toBinary(len);
    aux = Util::convertUIntToChar(len, tAux);
    this->out->write(aux,tAux);
    this->out->write(reinterpret_cast<char*>(tree), len);
    delete[] tree;

    tree = let->toBinary(len);
    aux = Util::convertUIntToChar(len, tAux);
    this->out->write(aux,tAux);
    this->out->write(reinterpret_cast<char*>(tree), len);
    delete[] tree;
}

void Compress::write(Patricia<Element *> *tags, Patricia<Element *> *attr, std::unordered_map<unsigned char, HuffBits> *num, std::unordered_map<unsigned char, HuffBits> *let)
{
    this->parser->reset();
    this->parser->compress(this->out, tags, attr, num, let);
    this->out->flush();
    this->out->close();
}

/*
IMPRESSÃO

    cout << "====LETRAS======" << endl;
    for(int k=0;k<257;k++){
        if(contLet[k]==0) continue;
        cout << (char) k << " " << contLet[k] << endl;
    }

    cout << "====NÚMEROS======" << endl;
    for(int k=0;k<257;k++){
        if(contNum[k]==0) continue;
        cout << (char) k << " " << contNum[k] << endl;
    }

    cout << "====TAGS======" << endl;
    for(Patricia<Element*>::iterator it = contTag->begin();
        it!=contTag->end();it++){
        string chave = (*it);
        Element *e = *(+it);
        cout << chave << " " << e->getAmount() << " " << e->getType() << endl;
    }

    cout << "====ATRIBUTOS======" << endl;

    for(Patricia<Element*>::iterator it = contAttr->begin();
        it!=contAttr->end();it++){
        string chave = (*it);
        Element *e = *(+it);
        cout << chave << " " << e->getAmount() << " " << e->getType() << endl;
    }

*/
