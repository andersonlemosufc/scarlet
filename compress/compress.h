#ifndef COMPRESS_H
#define COMPRESS_H

#include <fstream>

#include "./xml/streamparser.h"
#include "./comparable/huffman/huffelement.h"
#include "./queue/priorityqueue.h"

class Compress
{
public:
    Compress(QString &filename);
    static void compress(QString filename);
    void compress();


private:

    QString filename;
    HuffElement *createHuffElement(Patricia<Element*>* patricia, bool tags);
    HuffmanTree* createHuffTree(int *cont);
    void writeTrees(HuffElement* tags, HuffElement* attr, HuffmanTree* num, HuffmanTree* let);
    void write(Patricia<Element*>* tags, Patricia<Element*>* attr,
               std::unordered_map<unsigned char, HuffBits> *num, std::unordered_map<unsigned char, HuffBits> *let);
    StreamParser *parser;
    std::ofstream *out;
};

#endif // COMPRESS_H
