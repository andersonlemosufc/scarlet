#include "comparable.h"

Comparable::Comparable()
{

}

bool Comparable::equals(Comparable *other)
{
    return (this->compareTo(other)==0);
}


