#ifndef QUARTZ_PARSER_H
#define QUARTZ_PARSER_H

#include <vector>

#include "Token.h"
#include "ParserNode.h"

class Parser {
public:
    Parser(std::vector<Token> tokens);

    NodeProgram ParseProgram();
private:
    std::vector<Token> m_Tokens;
    int m_Index = 0;

    Token Peak(int offset = 0);
    Token Consume(int amount = 1);

    NodeExpr ParseExpr();
    NodeStatement ParseStatement();
};

#endif //QUARTZ_PARSER_H
