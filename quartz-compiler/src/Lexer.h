#ifndef QUARTZ_LEXER_H
#define QUARTZ_LEXER_H

#include <vector>

#include "Token.h"

class Lexer {
public:
    std::vector<Token> Tokenize(const std::string& data);
};


#endif //QUARTZ_LEXER_H
