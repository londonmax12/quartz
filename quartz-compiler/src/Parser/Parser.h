#ifndef QUARTZ_PARSER_H
#define QUARTZ_PARSER_H

#include <vector>

#include "../Lexer/Token.h"
#include "AstNode.h"
#include "../Utils/MemoryPool.h"

namespace Quartz {
    class ParserException : public std::exception {
    public:
        ParserException(const std::string& message, Token token = Token({TokenType::NONE}))
            : m_ErrorMessage(message), m_Token(token) {}

        const char* what() const noexcept override {
            return m_ErrorMessage.c_str();
        }
        Token GetToken() const { return m_Token; };
    private:
        std::string m_ErrorMessage;
        Token m_Token;
    };

    class Parser {
    public:
        Parser(std::vector<Token> tokens);
        ~Parser();

        NodeProgram ParseProgram();
    private:
        std::vector<Token> m_Tokens;
        int m_Index = 0;

        MemoryPool m_Pool;

        Token Peak(int offset = 0);
        Token Consume(int amount = 1);
        Token TryConsume(TokenType type, const std::string& errMessage);

        NodeExpr* ParseExpr(int minPrec = 0);
        NodeTerm* ParseTerm();
        NodeBinExpr* ParseBinExpr();
        NodeScope* ParseScope();
        NodeStatement* ParseStatement();
    };
}

#endif //QUARTZ_PARSER_H
