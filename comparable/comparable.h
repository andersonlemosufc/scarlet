#ifndef COMPARABLE_H
#define COMPARABLE_H


class Comparable
{
public:
    Comparable();
    virtual int compareTo(Comparable *other) = 0;
    bool equals(Comparable* other);
};

#endif // COMPARABLE_H
