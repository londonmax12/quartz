#include "Parser.h"

#include <iostream>

Parser::Parser(std::vector<Token> tokens)
    : m_Tokens(tokens)
{

}

Token Parser::Peak(int ahead) {
    if (m_Index + ahead > m_Tokens.size()) {
        return {TokenType::NONE};
    }
    return m_Tokens[m_Index];
}

Token Parser::Consume() {
    return m_Tokens[m_Index++];
}

ExitNode Parser::Parse() {
    ExitNode exitNode{};
    while (Peak().GetType() != TokenType::NONE)
    {
        switch (Peak().GetType()) {
            case TokenType::EXIT: {
                Consume();
                ExprNode exprNode = ParseExpr();
                if (exprNode.IntLiteral.GetType() != NONE ) {
                    exitNode = ExitNode(exprNode);
                }
                else {
                    std::cerr << "Invalid expression\n";
                    exit(1);
                }
                if (Peak().GetType() != TokenType::ENDL) {
                    std::cerr << "Expected semi colon\n";
                    exit(1);
                }
                break;
            }
            default: {
                Consume();
                break;
            }
        }
    }
    return exitNode;
}

ExprNode Parser::ParseExpr() {
    if (Peak().GetType() == TokenType::INT_LIT) {
        return ExprNode(Consume());
    }
    return ExprNode(Token{TokenType::NONE});
}
