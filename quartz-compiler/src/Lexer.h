#ifndef QUARTZ_LEXER_H
#define QUARTZ_LEXER_H

#include <vector>

#include "Token.h"

class Lexer {
public:
    Lexer(std::string src);
    std::vector<Token> Tokenize();

private:
    const std::string m_Src;
    int m_Index = 0;

    char Peak(int ahead = 1);
    char Consume();
};


#endif //QUARTZ_LEXER_H
