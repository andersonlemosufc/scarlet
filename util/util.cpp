#include "util.h"

using namespace std;

Util::Util()
{

}

Patricia<Element *> *Util::tags()
{
    Patricia<Element*> *patricia = new Patricia<Element*>();
    #include "./util/tagsForPat.h"
    return patricia;
}

std::unordered_map<int, Element *> *Util::mapOfTags()
{
    unordered_map<int,Element*>* map = new unordered_map<int,Element*>();
    #include "./util/tagsForMap.h"
    return map;
}


Patricia<Element *> *Util::attributes()
{
    Patricia<Element*> *patricia = new Patricia<Element*>();
    #include "./util/attrForPat.h"
    return patricia;
}

std::unordered_map<int, Element *> *Util::mapOfAttributes()
{

    unordered_map<int,Element*>* map = new unordered_map<int,Element*>();
    #include "./util/attrForMap.h"
    return map;
}

int Util::toInt(char *ch)
{
    unsigned char c = (unsigned char) ch[0];
    if(c<255) return c;
    else {
        int i = (int) c;
        c = (unsigned char) ch[1];
        return i+((int)c);
    }
}

unsigned char Util::getChar(char *ch, unsigned char *mask, int *index)
{
    unsigned char c = (unsigned char) ch[*index];
    unsigned char aux = 0;
    for(int m=128;m>0;m>>=1){
        (*mask)>>=1;
        if((*mask)==0){
            (*mask)=128;
            (*index)++;
            c = ch[(*index)];
        }
        if((*mask) & c) aux = aux | m;
    }
    return aux;
}

std::string Util::trim(QStringRef s)
{
    return s.toString().trimmed().toStdString();
}

QString Util::getWithoutExtension(QString filename)
{
    int k = filename.lastIndexOf("/");
    for(int t=filename.length();k<t;k++){
        if(filename[k]=='.') break;
    }
    return filename.left(k);
}

string Util::getOutputFilename(QString filename)
{
    return Util::getWithoutExtension(filename).replace("/entradas-comparativo/", "/scl/").toStdString() + ".scl";
    //return Util::getWithoutExtension(filename).toStdString()+".scl";
}

string Util::getDecompressedFilename(QString filename)
{
    return Util::getWithoutExtension(filename).replace("/scl/", "/decompressed/").toStdString() + ".sc.svg";
    //return Util::getWithoutExtension(filename).toStdString()+".sc.svg";
}

void Util::printIntBinaty(int n)
{
    int m = 1 << 31;
    for(int bit=1;bit<33;bit++){
        cout << ((n & m) ? "1" : "0");
        n <<=1;
        if(bit%8==0) cout << " ";
    }
    cout << endl;
}


void Util::binario(char *ch, int n)
{

    string s = "";
    for(int k=0;k<n;k++){
        for(unsigned char m=128;m>0;m>>=1){
            s+=((m & ch[k]) ? "1" : "0");
        }
        s+=" ";
    }
    cout << s << endl;
}


char *Util::convertUIntToChar(int n, int &len)
{
    if(n<0) return nullptr;
    if(n<128) len = 1;
    else if(n<16384) len = 2;
    else len = 4;
    char *res = new char[len];

    unsigned char m = 0;
    int index = len;
    int nn = (n<0) ? -n-1 : n;
    while(nn>0){
        if(m==0){
            m = 1;
            index--;
            res[index] = 0;
        }
        if(nn%2==1) res[index] = (m | res[index]);
        nn/=2;
        m<<=1;
    }
    index--;
    while(index>=0){res[index--]=0;}

    if(len>1){
        unsigned char m = (len==2) ? 128 : 192;
        res[0] = res[0] | m;
    }
    return res;
}



int Util::nextUInt(char *ch, int *firstPosition)
{
    int res = 0;
    unsigned char m = 128;
    int len = 1;

    int k = *firstPosition;
    if(m & ch[k]){
        m>>=1;
        if(m & ch[k]){
            len = 4;
        } else {
            len = 2;
        }
    }
    m>>=1;
    int aux = (len==1) ? 64 : (len==2) ? 8192 : 536870912;
    while(aux>0){
        if(m==0){
            m = 128;
            k++;
        }
        if(m & ch[k]) { res += aux;}
        m>>=1;
        aux/=2;
    }
    (*firstPosition)=k+1;
    return res;
}


int Util::nextUInt(ifstream *in)
{
    char c;
    in->read(&c, 1);
    return Util::nextUInt(c, in);
}

int Util::nextUInt(char c, ifstream *in)
{
    int res = 0;
    unsigned char m = 128;
    int len = 1;

    int k = 0;
    if(m & c){
        m>>=1;
        if(m & c){
            len = 4;
        } else {
            len = 2;
        }
    }
    char *ch = new char[len];
    ch[0] = c;
    if(len>1) in->read(&ch[1], len-1);
    m>>=1;
    int aux = (len==1) ? 64 : (len==2) ? 8192 : 536870912;
    while(aux>0){
        if(m==0){
            m = 128;
            k++;
        }
        if(m & ch[k]) { res += aux;}
        m>>=1;
        aux/=2;
    }
    return res;
}
