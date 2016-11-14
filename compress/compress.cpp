#include "compress.h"

using namespace std;

Compress::Compress(QString &filename)
{
    this->filename = filename;
    this->parser = new StreamParser(filename);
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

    this->out = new ofstream(Util::getOutputFilename(this->filename), ofstream::binary);
    *(this->out) << " ";
    this->writeTrees(huffTags, huffAttr, treeNum, treeLet);
    this->write(tableTags, tableAttr, tableNum, tableLet);
    this->out->close();
    delete out;

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
