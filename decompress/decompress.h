#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "./comparable/huffman/huffelement.h"
#include "./util/util.h"
#include <iostream>
#include <fstream>

#define READ_BUFFER_LENGHT int(4194304)//4megas
#define WRITE_BUFFER_LENGHT int(1048576)//1mega

class Decompress
{
public:
    Decompress(QString filename);
    static void decompress(QString filename);
    void decompress();


private:

    QString filename;


    char* dataIn;
    int numberOfBytesRead;
    int indexIn;
    unsigned char mask;
    unsigned char byte;
    std::ifstream *in;
    bool incompleteTag;
    std::stack<Element*> stack;
    int bitsDespisedContent;
    bool lastWasContent{false};

    char* buffer;
    int indexBuffer;
    std::ofstream *out;
    HuffElement *tags;
    HuffElement *attrs;
    HuffmanTree *numbers;
    HuffmanTree *letters;

    bool readTag(HuffElement *tags);
    void readAttr(HuffElement *attrs);
    bool readSeparator();
    bool readCont(bool isNumber);

    int readInt();
    inline void increaseMask(){
        this->mask>>=1;
        if(this->mask==0){
            this->mask = 128;
            this->indexIn++;
            if(this->indexIn==READ_BUFFER_LENGHT){
                qDebug() << "error";
                this->indexIn=0;
                this->in->read(this->dataIn, READ_BUFFER_LENGHT);
                this->numberOfBytesRead = this->in->gcount();
            }
            this->byte = this->dataIn[this->indexIn];
        }
    }


    inline bool readBit(){
        bool res = (byte & mask);
        this->increaseMask();
        return res;
    }

    inline void closeTag(){
        Element *e = stack.top();
        stack.pop();
        switch(e->getToken()){
        case Element::DTD: break;
        case Element::COMMENT: *out << " -->\n"; break;
        case Element::START_DOCUMENT: *out << " ?>\n"; break;
            default: {
                if(e->hasContent()){
                    for(int i=0,t=stack.size();i<t;i++) *out << "\t";
                    *out << "</" << *(e->getName()) << ">\n";
                } else {
                    *out << " />\n";
                }
            }
        }
    }

};

#endif // DECOMPRESS_H
