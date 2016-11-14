#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "./comparable/huffman/huffelement.h"
#include "./util/util.h"
#include <iostream>
#include <fstream>

#define BUFFER_LENGHT int(4194304)//4megas

class Decompress
{
public:
    Decompress(QString filename);
    static void decompress(QString filename);
    void decompress();

private:
    QString filename;
    char* buffer;
    int indexBuffer;
};

#endif // DECOMPRESS_H
