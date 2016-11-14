#include "mainwindow.h"
#include <QApplication>
#include <iostream>

#include "./compress/compress.h"
#include "./decompress/decompress.h"

using namespace std;
void print(string *s);
void println(string *s);

QString getNome(int n){
    QString rr = "/home/anderson/Documentos/tcc/svg/";
    if(n<10) rr+="000";
    else if(n<100) rr+="00";
    else if(n<1000) rr+="0";
    rr = rr.append(QString("%1.svg").arg(n));
    return rr;
}


int main(){

    int op = 1;

    QStringList l;
    l.append("0001.svg");
    l.append("brasil.svg");

    QString filename = "/home/anderson/Documentos/tmp/scarlet-testes/"+l.at(op-1);

    Compress::compress(filename);
    Decompress::decompress(Util::getWithoutExtension(filename)+".scl");
}

/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
*/

void print(string *s){
    if(s==nullptr) cout << "NULL";
    else cout << *s;
}

void println(string *s){
    if(s==nullptr) cout << "NULL" << endl;
    else cout << *s << endl;
}

