#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>

typedef struct Bits{
    std::string value;
    int length;
} Bits;

typedef struct Code{
    char *code;
    int len;
    public :
        Code(int code){
            if(code<255) {
                len=1;
                this->code = new char[len];
                this->code[0] = code;
            } else {
                len=2;
                this->code = new char[len];
                this->code[0] = 255;
                this->code[1] = (code-255);
            }
        }
        Code(char* code, int len){
            this->len = len;
            this->code = new char[len];
            for(int k=0;k<len;k++) this->code[k] = code[k];
        }
        Code(const std::string& code){
            this->len = code.length();
            this->code = new char[len];
            for(int k=0;k<len;k++) this->code[k] = code[k];
        }
        ~Code(){
            delete[] code;
        }
} Code;

class Element
{

public:
    Element(std::string name);
    Element(std::string name, int amount);
    Element(std::string name, int amount, int token);
    Element(std::string name, Code *code, int token);
    Element(std::string name, Code *code, int type, int token);
    ~Element();

    static const int NUMBER = 0;
    static const int TEXT = 1;

    static const int NORMAL = 0;
    static const int START_DOCUMENT = 1;
    static const int DTD = 2;
    static const int COMMENT = 3;
    static const int ATTRIBUTE = 4;

    void setContent(bool flag);
    bool hasContent();
    void setCode(Code *code);
    void setName(std::string name);
    void setType(int type);
    void setAmount(int amount);
    Code* getCode();
    std::string *getName();
    int getType();
    int getAmount();
    void increaseAmount();

    Bits *getBits();
    void setBits(Bits value);

    int getToken();
    void setToken(int value);

    void setTokenByChar(char ch);

private:
    Code *code;
    Bits bits;
    std::string name;
    int type;
    int amount;
    int token;

    bool content{false};
};

#endif // ELEMENT_H
