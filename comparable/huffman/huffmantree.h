#ifndef NODOHUFFMAN_H
#define NODOHUFFMAN_H

#include <QString>
#include <QDebug>
#include <QMap>
#include <iostream>
#include <math.h>
#include <unordered_map>
#include "../comparable.h"


typedef struct {
    std::string value;
    int length;
} HuffBits;

class HuffmanTree : public Comparable
{
public:
    HuffmanTree();
    HuffmanTree(unsigned char value, int frequence);

private:
    int frequence;
    unsigned char value;
    HuffmanTree* right;
    HuffmanTree* left;
    int size;
    int height;
    int numberOfLeafs;

public:
    void setFrequence(int frequence);
    void setValue(unsigned char value);
    unsigned char getValue();
    int getFrequence();
    int compareTo(Comparable *other);
    void setLeft(HuffmanTree *left);
    void setRight(HuffmanTree *right);
    HuffmanTree *getLeft();
    HuffmanTree* getRight();
    QString toString();
    int getHeight();
    int getSize();
    bool isLeaf();
    bool equals(HuffmanTree *other);
    void print();
    int getNumberOfLeafs();
    std::unordered_map<unsigned char, HuffBits> *toDictionary();
    virtual unsigned char* toBinary(int &tam);
    static HuffmanTree* buildTree(char *structure);

protected:
    virtual void updateVariables();
    inline static void increaseMask(unsigned char *mask, int *index){
        (*mask)>>=1;
        if((*mask)==0){
            (*mask)=128;
            (*index)++;
        }
    }

private:
    void print(HuffmanTree *nodo, int t);
    void toDictionary(HuffmanTree* nodo, std::unordered_map<unsigned char, HuffBits> *mapa, HuffBits *aux, unsigned char *mask, int nivel);
    virtual void toBinary(HuffmanTree* nodo, unsigned char* vector, unsigned char *mask, int *index);
    static HuffmanTree* buildTree(char *structure, int* index, unsigned char* mask);

};

#endif // NODOHUFFMAN_H
