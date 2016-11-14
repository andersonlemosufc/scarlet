#ifndef UTIL_H
#define UTIL_H
#include <QString>
#include <string>
#include <iostream>
#include <fstream>
#include "./xml/element.h"
#include "./tree/patricia.h"


class Util
{
private:

public:
    Util();

    static Patricia<Element*>* tags();
    static Patricia<Element*>* attributes();

    static std::unordered_map<int,Element*>* mapOfTags();
    static std::unordered_map<int,Element*>* mapOfAttributes();

    static int toInt(char* ch);
    static unsigned char getChar(char* ch, unsigned char *mask, int *index);

    static std::string trim(QStringRef s);
    static QString getWithoutExtension(QString filename);
    static std::string getOutputFilename(QString filename);
    static std::string getDecompressedFilename(QString filename);
    static void printIntBinaty(int n);
    static void binario(char *ch, int n);

    static char* convertUIntToChar(int n, int &len); //turn len to the lenght of the return
    static int nextUInt(std::ifstream* in);
    static int nextUInt(char first, std::ifstream* in);
    static int nextUInt(char *ch, int* firstPosition);


};

#endif // UTIL_H

