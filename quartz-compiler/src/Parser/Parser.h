#ifndef QUARTZ_PARSER_H
#define QUARTZ_PARSER_H

#include <vector>

#include "../Lexer/Token.h"
#include "AstNode.h"
#include "../Utils/MemoryPool.h"

namespace Quartz {
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

        NodeExpr* ParseExpr(int minPrec = 0);
        NodeTerm* ParseTerm();
        NodeBinExpr* ParseBinExpr();
        NodeScope* ParseScope();
        NodeStatement* ParseStatement();
    };
}

#endif //QUARTZ_PARSER_H
