#ifndef QUARTZ_PARSERNODE_H
#define QUARTZ_PARSERNODE_H

#include "Token.h"

struct ExprNode {
    ExprNode() {};
    ExprNode(Token intLiteral)
            : IntLiteral(intLiteral) {};

    Token IntLiteral;
};
struct ExitNode {
    ExitNode() {};
    ExitNode(ExprNode expr)
            : Expr(expr) {};
    ExprNode Expr;
};

#endif //QUARTZ_PARSERNODE_H
