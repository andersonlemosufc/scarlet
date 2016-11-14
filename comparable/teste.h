#ifndef TESTE_H
#define TESTE_H

#include "comparable.h"

class Teste : public Comparable
{
public:
    Teste();
    Teste(int a);

    int a;
    int compareTo(Comparable *other);
};

#endif // TESTE_H
