#ifndef QUARTZ_PARSERNODE_H
#define QUARTZ_PARSERNODE_H

#include "Token.h"

#include <variant>
#include <vector>

struct NodeEmpty {};

struct NodeExprIntLit {
    NodeExprIntLit() {};
    NodeExprIntLit(Token intLiteral)
        : IntLiteral(intLiteral) {};

    Token IntLiteral;
};
struct NodeExprIdentifier {
    NodeExprIdentifier() {};
    NodeExprIdentifier(Token identifier)
        : Identifier(identifier) {};

    Token Identifier;
};
struct NodeExpr {
    NodeExpr() {};
    NodeExpr(std::variant<NodeEmpty, NodeExprIntLit, NodeExprIdentifier> expr)
        : Expr(expr) {};

    std::variant<NodeEmpty, NodeExprIntLit, NodeExprIdentifier> Expr;
};

struct NodeStatementExit {
    NodeStatementExit() {};
    NodeStatementExit(NodeExpr expr)
            : Expr(expr) {};
    NodeExpr Expr;
};

struct NodeStatementVarDecl {
    NodeStatementVarDecl() {};
    NodeStatementVarDecl(Token identifier, NodeExpr expr)
        : Identifier(identifier), Expr(expr) {};

    Token Identifier;
    NodeExpr Expr;
};

struct NodeStatement {
    NodeStatement() {};
    NodeStatement(std::variant<NodeEmpty, NodeStatementExit, NodeStatementVarDecl> statement)
        : Statement(statement) {};
    std::variant<NodeEmpty, NodeStatementExit, NodeStatementVarDecl> Statement;
};

struct NodeProgram {
    std::vector<NodeStatement> Statements;
};

#endif //QUARTZ_PARSERNODE_H
