#ifndef QUARTZ_LEXER_H
#define QUARTZ_LEXER_H

#include <vector>

#include "Token.h"

namespace Quartz {
    class LexerException : public std::exception {
    public:
        LexerException(const std::string& message, Token token = Token({TokenType::NONE}))
                : m_ErrorMessage(message), m_Token(token) {}

        const char* what() const noexcept override {
            return m_ErrorMessage.c_str();
        }
        Token GetToken() const { return m_Token; };
    private:
        std::string m_ErrorMessage;
        Token m_Token;
    };

    class Lexer {
    public:
        struct LexerOut {
            std::string FilePath;
            std::vector<Token> Tokens;
        };

        Lexer(std::string filePath);
        LexerOut Tokenize();

    private:
        LexerOut m_Out;

        const std::string m_FilePath;
        std::string m_Src;
        int m_Index = 0;

        int m_Line = 1;
        int m_Column = 1;

        char Peak(int offset = 0);
        char Consume();
    };
}

#endif //QUARTZ_LEXER_H
