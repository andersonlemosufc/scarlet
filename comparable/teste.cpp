#include "teste.h"

Teste::Teste()
{

}

Teste::Teste(int a)
{
    this->a = a;
}

int Teste::compareTo(Comparable *other)
{
    if(other==nullptr) return -1;
    Teste *t = (Teste*) other;
    return this->a - t->a;
}

