#ifndef QUARTZ_TOKEN_H
#define QUARTZ_TOKEN_H

#include <string>

enum TokenType {
    EXIT,
    INT_LIT,
    ENDL,
    OPEN_PAREN,
    CLOSE_PAREN,
    IDENTIFIER,
    VAR,
    VAR_INT,
    EQUALS,
    PLUS,
    MULTI,
    COLON,
    SUB,
    DIV,
    NONE
};

class Token {
public:
    Token() = default;
    Token(TokenType type, std::string value = "");

    TokenType GetType() { return m_Type; }
    std::string GetValue() { return m_Value; }

    bool IsValid();

    bool IsBinOperator();
    static bool IsBinOperator(TokenType type);

    int GetBinPrec();
    static int GetBinPrec(TokenType type);

    bool HasBinPrec();

    std::string Str();
private:
    TokenType m_Type;
    std::string m_Value;
};

#endif //QUARTZ_TOKEN_H
