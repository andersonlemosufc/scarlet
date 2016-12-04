#include "element.h"
#include <iostream>
using namespace std;

Element::Element(string name)
{
    this->name = name;
    this->code = nullptr;
    this->amount = 0;
    this->type = Element::TEXT;
    this->setTokenByChar(name[0]);
}

Element::Element(string name, int amount)
{
    this->name = name;
    this->code = nullptr;
    this->amount = amount;
    this->type = Element::TEXT;
    this->setTokenByChar(name[0]);
}

Element::Element(std::string name, int amount, int token)
{
    this->token = token;
    this->amount = amount;
    this->name = name;
    this->code = nullptr;
    this->type = Element::TEXT;
}

Element::Element(string name, Code *code, int token)
{
    this->token = token;
    this->name = name;
    this->code = code;
    this->amount = 0;
    this->type = Element::TEXT;
}


Element::Element(std::string name, Code *code, int type, int token)
{
    this->name = name;
    this->amount = 0;
    this->code = code;
    this->type = type;
    this->token = token;
}

Element::~Element()
{
    delete this->code;
}

bool Element::hasContent(){
    return this->content;
}

void Element::setContent(bool flag)
{
    this->content = flag;
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
int Element::getToken()
{
    return token;
}

void Element::setToken(int value)
{
    token = value;
}

void Element::setTokenByChar(char ch)
{
    switch (ch) {
        case '?': this->token = Element::START_DOCUMENT; break;
        case '!': this->token = Element::DTD; break;
        case '-': this->token = Element::COMMENT; break;
        default: this->token = Element::NORMAL; break;
    }
}

bool Element::equals(Element *other)
{
    if(other==nullptr) return false;
    if(this->name != other->name) return false;
    if(this->code->len!=other->code->len) return false;
    for(int k=0;k<this->code->len;k++){
        if(this->code->code[k]!=other->code->code[k]) return false;
    }
    if(this->type != other->type) return false;
    if(this->token != other->token) return false;
    return true;
}



