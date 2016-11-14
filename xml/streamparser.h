#ifndef STREAMPARSER_H
#define STREAMPARSER_H

#include <QString>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>


#include <iostream>
#include <unordered_map>
#include <stack>
#include <fstream>

#include "./tree/patricia.h"
#include "./util/util.h"
#include "./comparable/huffman/huffelement.h"

#define BUFFER_LENGHT int(8388608) //8megas

class StreamParser
{

public:
    StreamParser(QString filename);
    ~StreamParser();
    void countFrequence(int *contNumbers, int* conLetters, Patricia<Element*>* patTags, Patricia<Element*>* patAtr);
    void reset();
    void compress(std::ofstream* out, Patricia<Element *> *tags, Patricia<Element *> *attr, std::unordered_map<unsigned char, HuffBits> *num, std::unordered_map<unsigned char, HuffBits> *let);
    /*

    bool treatNextTag();
    bool isEnd();
    int compress(char **compressed, char *data, long len);
    int uncompress(char **uncompressed, char *data, long len);*/

private:
    QXmlStreamReader xml;
    QFile* xmlFile;
    bool end;
    std::stack<int> types;

    std::ofstream *out;
    char *buffer;
    int lenghtBuffer;
    int contSeparator;
    unsigned char mask;
    unsigned char byte;

    void startElement();
    void endElement();
    void characters();
    void comment();
    void dtd();
    void startDocument();
    void endDocument();
    void invalid();

    void contChar(const QStringRef content, int *cont);
    int contTag(const QStringRef name, const QStringRef prefix, Patricia<Element*>* pat);
    int contAtt(const QStringRef name, const QStringRef prefix, const QStringRef content, int *contNumbers, int* conLetters, Patricia<Element*>* pat);


    void writeSeparator(bool isTag);
    int writeTag(QStringRef name, QStringRef prefix, Patricia<Element*>*pat);
    int writeAttr(const QStringRef name, const QStringRef prefix, const QStringRef content,
                std::unordered_map<unsigned char,HuffBits>*num, std::unordered_map<unsigned char,
                HuffBits>* let, Patricia<Element*>* pat);
    void writeChar(std::string content, std::unordered_map<unsigned char,HuffBits>* map, bool isAtrr);
    inline void increaseMask(){
        this->mask>>=1;
        if(mask==0){
            this->mask = 128;
            this->buffer[this->lenghtBuffer++] = this->byte;
            this->byte = 0;
            if(this->lenghtBuffer==BUFFER_LENGHT){
                this->out->write(this->buffer, this->lenghtBuffer);
                this->lenghtBuffer = 0;
            }
        }
    }
    inline void writeBit(){
        this->byte = this->byte & this->mask;
        this->increaseMask();
    }
    inline void writeBit(bool ok){
        if(ok) this->byte = this->byte & this->mask;
        this->increaseMask();
    }
};

#endif // STREAMPARSER_H
