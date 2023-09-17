#ifndef QUARTZ_TOKEN_H
#define QUARTZ_TOKEN_H

#include <string>

enum TokenType {
    RETURN,
    INT_LIT,
    ENDL
};

class Token {
public:
    Token(TokenType type, std::string value = "");

    TokenType m_Type;
    std::string m_Value;

    std::string Str();
};

#endif //QUARTZ_TOKEN_H
