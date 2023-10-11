#ifndef QUARTZ_TOKEN_H
#define QUARTZ_TOKEN_H

#include <string>
#include <ostream>

namespace Quartz {
    enum TokenType {
        EXIT,
        INT_LIT,
        ENDL,
        OPEN_PAREN,
        CLOSE_PAREN,
        OPEN_CURLY,
        CLOSE_CURLY,
        IDENTIFIER,
        VAR,
        VAR_INT,
        EQUALS,
        PLUS,
        MULTI,
        COLON,
        SUB,
        DIV,
        IF,

        ERROR,
        NONE
    };

    class Token {
    public:
        Token() = default;
        Token(TokenType type, std::string value = "");
        Token(TokenType type, int line, int col, std::string value = "");

        TokenType GetType() const { return m_Type; }
        std::string GetValue() const { return m_Value; }

        int GetLine() const { return m_Line; }
        int GetColumn() const { return m_Column; }

        bool IsValid();

        bool IsBinOperator();
        static bool IsBinOperator(TokenType type);

        int GetBinPrec();
        static int GetBinPrec(TokenType type);

        bool HasBinPrec();

        friend std::ostream& operator<<(std::ostream& os, const Token& token);
    private:
        TokenType m_Type;
        std::string m_Value;

        int m_Line = 0;
        int m_Column = 0;
    };
}

#endif //QUARTZ_TOKEN_H
