#include "huffelement.h"

using namespace std;

HuffElement::HuffElement()
{
    this->numberOfCaracteres = 0;
}

HuffElement::HuffElement(Element *element, int frequence) :
    HuffmanTree(-1, frequence)
{
    this->element = element;
    this->numberOfCaracteres = (element==nullptr) ? 0 : (element->getCode()==nullptr) ? element->getName()->size()+1 : element->getCode()->len;
}

void HuffElement::setElement(Element *element)
{
    this->element = element;
}

Element *HuffElement::getElement()
{
    return this->element;
}

void HuffElement::updateVariables()
{
    HuffmanTree::updateVariables();
    int l=0,r=0;
    if(this->getLeft()!=nullptr) l = ((HuffElement*) this->getLeft())->numberOfCaracteres;
    if(this->getRight()!=nullptr) r = ((HuffElement*) this->getRight())->numberOfCaracteres;
    this->numberOfCaracteres = (l+r);
}


Patricia<Element*> *HuffElement::toDictionary()
{
    Patricia<Element*>* patricia = new Patricia<Element*>();
    int tam = ceil(this->getHeight()/8.0);
    char *c = new char[tam];
    for(int k=0;k<tam;k++) c[k] = 0;
    Bits b = {std::string(c,tam), 0};
    unsigned char mask = 0;
    this->toDictionary(this, patricia, &b, &mask, 0, "");
    return patricia;
}

void HuffElement::toDictionary(HuffmanTree *nodo, Patricia<Element *> *patricia, Bits *aux, unsigned char *mask, int nivel, std::string str)
{
    (*mask)>>=1;
    if((*mask)==0) *mask = 128;
    if(nodo->isLeaf()){
        aux->length = nivel;
        Element *e = ((HuffElement*)nodo)->getElement();
        e->setBits(*aux);
        patricia->insert(*(e->getName()), e);
    } else {
        int index = nivel/8;

        aux->value[index] = aux->value[index] & ~(*mask);
        this->toDictionary(nodo->getLeft(), patricia, aux, mask, nivel+1, str+"0");
        (*mask)<<=1;
        if((*mask)==0) *mask = 1;

        aux->value[index] = (aux->value[index] | (*mask));
        this->toDictionary(nodo->getRight(), patricia, aux, mask, nivel+1, str+"1");
        (*mask)<<=1;
        if((*mask)==0) *mask = 1;
    }
}


unsigned char *HuffElement::toBinary(int &tam)
{
    tam = this->numberOfCaracteres+ceil(this->getNumberOfLeafs()/8.0)+ceil(this->getSize()/8.0);
    unsigned char* res = new unsigned char[tam];
    for(int k=0;k<tam;k++) res[k] = 0;
    unsigned char mask = 128;
    int index = 0;
    this->toBinary(this,res,&mask,&index);
    return res;
}


void HuffElement::toBinary(HuffmanTree *nodo, unsigned char *vector, unsigned char *mask, int *index)
{
    if(nodo->isLeaf()){
        vector[(*index)] = vector[(*index)] | (*mask);
        this->increaseMask(mask, index);
        Element *e = ((HuffElement*)nodo)->getElement();
        int t=0;
        unsigned char* data;
        if(e->getCode()==nullptr){
            data = (unsigned char*) e->getName()->c_str();
            t = e->getName()->length()+1;
        } else {
            data = (unsigned char*) e->getCode()->code;
            t = e->getCode()->len;
            vector[(*index)] = vector[(*index)] | (*mask);
        }
        this->increaseMask(mask, index);
        for(int i=0;i<t;i++){
            for(unsigned char k=128;k>0;k>>=1){
                if(k & data[i])
                    vector[(*index)] = vector[(*index)] | (*mask);
                this->increaseMask(mask, index);
            }
        }
    } else {
        this->increaseMask(mask, index);
        this->toBinary(nodo->getLeft(),vector,mask,index);
        this->toBinary(nodo->getRight(),vector,mask,index);
    }
}


HuffElement *HuffElement::buildTree(char *structure, bool tags)
{
    unordered_map<int,Element*>* mapa = (tags) ? Util::mapOfTags() : Util::mapOfAttributes();
    int index = -1;
    unsigned char mask = 0;
    return HuffElement::buildTree(structure,&index,&mask,mapa);
}


HuffElement *HuffElement::buildTree(char *structure, int *index, unsigned char *mask, std::unordered_map<int,Element*>* ele)
{
    HuffElement* nodo = new HuffElement();
    nodo->setElement(nullptr);
    (*mask)>>=1;
    if((*mask)==0){
        (*mask)=128;
        (*index)++;
    }
    unsigned char ch = (unsigned char) structure[(*index)];
    //se e folha
    if((*mask) & ch){
        increaseMask(mask, index);
        //se esta no mapa
        if((*mask) & ch){
            int i = ((int) Util::getChar(structure, mask, index));
            if(i==255) i += ((int)Util::getChar(structure, mask, index));
            Element *e = ele->at(i);
            nodo->setElement(e);
        } else {
            std::vector<unsigned char> v;
            ch = Util::getChar(structure, mask, index);
            while(ch!=0){
                v.push_back(ch);
                ch = Util::getChar(structure, mask, index);
            }
            v.push_back(ch);
            Element *e = new Element(string((char*)v.data()));
            nodo->setElement(e);
        }
    } else {
        nodo->setLeft(HuffElement::buildTree(structure, index, mask, ele));
        nodo->setRight(HuffElement::buildTree(structure, index, mask, ele));
    }
    return nodo;
}


QString HuffElement::toString()
{
    QString res = "";
    res.append("[valor: ").append(((this->element!=nullptr) ?
                                       QString::fromStdString(*(this->element->getName()))
                                  : "NULO")).append(", ");
    res.append(QString("frequencia: %1").arg(this->getFrequence())).append(", ");
    res.append("left: ").append(this->getLeft()==nullptr ? "NULO": "NAO NULO").append(", ");
    res.append("right: ").append(this->getRight()==nullptr ? "NULO": "NAO NULO").append("]");
    return res;
}

void HuffElement::print()
{
    this->print(this, 0);
}

void HuffElement::print(HuffmanTree* nodo, int t)
{
    if(nodo!=nullptr){
        HuffElement* he = (HuffElement*) nodo;
        QString s = QString("%1").arg(t);
        for(int k=0;k<t;k++)s+="    ";
        s = QString(s.append("val: ").append(((he->element!=nullptr) ?
                                                  QString::fromStdString(*(he->element->getName()))
                                             : "NULO")).append(" > freq: %1")).arg(nodo->getFrequence());
        qDebug() << s;
        this->print(nodo->getLeft(), t+1);
        this->print(nodo->getRight(), t+1);
    }
}

