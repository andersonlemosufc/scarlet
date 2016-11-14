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
    this->buffer = new char[BUFFER_LENGHT];
    this->indexBuffer = 0;
    ifstream in(filename.toStdString());
    char ch;
    in.read(&ch, 1);
    int bitsDespisedContent = ch-'0';

    int len = Util::nextUInt(&in);
    in.read(buffer, len);
    HuffElement *tags = HuffElement::buildTree(buffer, true);

    len = Util::nextUInt(&in);
    in.read(buffer, len);
    HuffElement *attr = HuffElement::buildTree(buffer, false);

    len = Util::nextUInt(&in);
    in.read(buffer, len);
    HuffmanTree *num = HuffmanTree::buildTree(buffer);

    len = Util::nextUInt(&in);
    in.read(buffer, len);
    HuffmanTree *let = HuffmanTree::buildTree(buffer);

    tags->print();
    attr->print();
    num->print();
    let->print();

    bool over = false;
    while(!in.eof() && !over){
        in.read(buffer, BUFFER_LENGHT);
        int tam = in.gcount();
        for(int k=0;k<tam && !over;k++){
            unsigned char ch = (unsigned char) buffer[k];
            unsigned char m=128;
            for(int i=0;i<8;i++,m>>=1){
                if(in.eof() && k==(tam-1) && i>=(8-bitsDespisedContent)){
                    flag = false;
                    break;
                }

            }
        }
    }
}

