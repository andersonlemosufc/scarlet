#ifndef HUFFELEMENT_H
#define HUFFELEMENT_H

#include<iostream>
#include<QDebug>

#include "./comparable/huffman/huffmantree.h"
#include "./xml/element.h"
#include "./tree/patricia.h"
#include "./util/util.h"

class HuffElement : public HuffmanTree
{
public:
    HuffElement();
    HuffElement(Element* element, int frequence);

    void setElement(Element* element);
    Element* getElement();

    Patricia<Element*>* toDictionary();
    unsigned char* toBinary(int &tam);
    static HuffElement* buildTree(char *structure, bool tags);
    QString toString();
    void print();

private:
    Element *element;
    int numberOfCaracteres;

    void print(HuffmanTree *nodo, int t);
    void updateVariables();
    void toDictionary(HuffmanTree* nodo, Patricia<Element*>* patricia,
                      Bits *aux, unsigned char *mask, int nivel, std::string str);

    void toBinary(HuffmanTree *nodo, unsigned char *vector,
                               unsigned char *mask, int *index);
    static HuffElement* buildTree(char *structure, int* index, unsigned char* mask, std::unordered_map<int, Element *> *ele);
};

#endif // HUFFELEMENT_H
