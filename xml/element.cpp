#include "element.h"
#include <iostream>
using namespace std;

Element::Element(string name)
{
    this->name = name;
    this->code = nullptr;
    this->amount = 0;
    this->type = Element::TEXT;
}

Element::Element(std::string name, int amount)
{
    this->amount = amount;
    this->name = name;
    this->code = nullptr;
    this->type = Element::TEXT;
}

Element::Element(string name, Code *code)
{
    this->name = name;
    this->code = code;
    this->amount = 0;
    this->type = Element::TEXT;
}


Element::Element(std::string name, Code *code, int type)
{
    this->name = name;
    this->amount = 0;
    this->code = code;
    this->type = type;
}

Element::~Element()
{
    delete this->code;
}

void Element::setCode(Code *code)
{
    this->code = code;
}

void Element::setName(string name)
{
    this->name = name;
}

void Element::setType(int type)
{
    this->type = type;
}

void Element::setAmount(int amount)
{
    this->amount = amount;
}

Code *Element::getCode()
{
    return this->code;
}

std::string* Element::getName()
{
    return &(this->name);
}

int Element::getType()
{
    return this->type;
}

int Element::getAmount()
{
    return this->amount;
}

void Element::increaseAmount()
{
    this->amount++;
}
Bits* Element::getBits()
{
    return &bits;
}

void Element::setBits(Bits value)
{
    bits = value;
}


