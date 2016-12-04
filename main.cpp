#include "mainwindow.h"
#include <QApplication>
#include <iostream>

#include "./compress/compress.h"
#include "./decompress/decompress.h"

#include <chrono>

using namespace std::chrono;
using namespace std;

QString getNome(int n, bool compress){
    QString aux = (compress) ? "entradas-comparativo" : "scl";
    QString rr = "/home/anderson/Documentos/tcc/"+aux+"/";
    if(n<10) rr+="000";
    else if(n<100) rr+="00";
    else if(n<1000) rr+="0";
    rr = (compress) ? rr.append(QString("%1.svg").arg(n)) : rr.append(QString("%1.scl").arg(n));
    return rr;
}


int main(){
    high_resolution_clock::time_point t1, t2;
    int exe = 5;
    QString name = "/home/anderson/Documentos/tcc/tempo" + QString("%1").arg(exe)+ ".txt";
    ofstream out(name.toStdString());
   for(int k=1;k<=3000;k++){
        QString filename = getNome(k, false);
        t1 = high_resolution_clock::now();
        //Compress::compress(filename);
        Decompress::decompress(filename);
        t2 = high_resolution_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
        out << t << "\n";
    }
   out.close();
}
