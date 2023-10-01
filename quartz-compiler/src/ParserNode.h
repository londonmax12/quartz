#ifndef QUARTZ_PARSERNODE_H
#define QUARTZ_PARSERNODE_H

#include "Token.h"

#include <variant>
#include <vector>

struct NodeEmpty {};

struct NodeExpr;

struct NodeBinExprAdd {
    NodeExpr* Lhs;
    NodeExpr* Rhs;
};

struct NodeBinExpr {
    std::variant<NodeEmpty*, NodeBinExprAdd*> Expr;
};

struct NodeTermIntLit {
    NodeTermIntLit() = default;
    NodeTermIntLit(Token intLiteral)
            : IntLiteral(intLiteral) {};

    Token IntLiteral{TokenType::NONE};
};

struct NodeTermIdentifier {
    NodeTermIdentifier() = default;
    NodeTermIdentifier(Token identifier)
            : Identifier(identifier) {};

    Token Identifier{TokenType::NONE};;
};

struct NodeTerm {
    std::variant<NodeTermIntLit*, NodeTermIdentifier*> Term;
};

struct NodeExpr {
    NodeExpr() = default;
    NodeExpr(std::variant<NodeEmpty*, NodeTerm*, NodeBinExpr*> expr)
        : Expr(expr) {};

    std::variant<NodeEmpty*, NodeTerm*, NodeBinExpr*> Expr;
};

struct NodeStatementExit {
    NodeStatementExit() = default;
    NodeStatementExit(NodeExpr* expr)
            : Expr(expr) {};

    NodeExpr* Expr;
};

struct NodeStatementVarDecl {
    NodeStatementVarDecl() = default;
    NodeStatementVarDecl(Token identifier, NodeExpr* expr)
        : Identifier(identifier), Expr(expr) {};

    Token Identifier;
    NodeExpr* Expr;
};

struct NodeStatement {
    NodeStatement() = default;
    NodeStatement(std::variant<NodeEmpty*, NodeStatementExit*, NodeStatementVarDecl*> statement)
        : Statement(statement) {};
    std::variant<NodeEmpty*, NodeStatementExit*, NodeStatementVarDecl*> Statement;
};

struct NodeProgram {
    std::vector<NodeStatement> Statements;
};

#endif //QUARTZ_PARSERNODE_H
