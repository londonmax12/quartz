#ifndef QUARTZ_PARSER_H
#define QUARTZ_PARSER_H

#include <vector>

#include "Token.h"
#include "ParserNode.h"

class Parser {
public:
    Parser(std::vector<Token> tokens);

    ExprNode ParseExpr();
    ExitNode Parse();
private:
    std::vector<Token> m_Tokens;
    int m_Index = 0;

    Token Peak(int ahead = 1);
    Token Consume();
};

#endif //QUARTZ_PARSER_H
