#ifndef QUARTZ_ASTNODE_H
#define QUARTZ_ASTNODE_H

#include "../Lexer/Token.h"

#include <variant>
#include <vector>

namespace Quartz {
    struct NodeEmpty {
    };

    struct NodeExpr;

    struct NodeBinExprAdd {
        NodeExpr* Lhs;
        NodeExpr* Rhs;
    };

    struct NodeBinExprMulti {
        NodeExpr* Lhs;
        NodeExpr* Rhs;
    };

    struct NodeBinExprSub {
        NodeExpr* Lhs;
        NodeExpr* Rhs;
    };

    struct NodeBinExprDiv {
        NodeExpr* Lhs;
        NodeExpr* Rhs;
    };

    struct NodeBinExpr {
        std::variant<NodeEmpty* , NodeBinExprAdd*, NodeBinExprSub*, NodeBinExprMulti*, NodeBinExprDiv*> Expr;
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

    struct NodeTermParen {
        NodeTermParen() = default;
        NodeTermParen(NodeExpr *expr)
                : Expr(expr) {};

        NodeExpr* Expr;
    };

    struct NodeTerm {
        NodeTerm() = default;
        NodeTerm(std::variant<NodeTermIntLit*, NodeTermIdentifier*, NodeTermParen*> term)
        : Term(term) {};

        std::variant<NodeTermIntLit*, NodeTermIdentifier*, NodeTermParen*> Term;
    };

    struct NodeExpr {
        NodeExpr() = default;
        NodeExpr(std::variant<NodeEmpty*, NodeTerm*, NodeBinExpr*> expr)
                : Expr(expr) {};

        std::variant<NodeEmpty*, NodeTerm*, NodeBinExpr*> Expr;
    };

    struct NodeStatement;

    struct NodeScope {
        std::vector<NodeStatement*> Statements;
    };

    struct NodeStatementExit {
        NodeStatementExit() = default;
        NodeStatementExit(NodeExpr *expr)
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

    struct NodeStatementIf {
        NodeStatementIf() = default;
        NodeStatementIf(NodeExpr* expr, NodeStatement* statement)
            : Statement(statement), Expr(expr) {};

        NodeExpr* Expr;
        NodeStatement* Statement;
    };

    struct NodeStatement {
        NodeStatement() = default;
        NodeStatement(std::variant<NodeEmpty*, NodeStatementExit*, NodeStatementVarDecl*, NodeScope*, NodeStatementIf*> statement)
                : Statement(statement) {};
        std::variant<NodeEmpty*, NodeStatementExit*, NodeStatementVarDecl*, NodeScope*, NodeStatementIf*> Statement;
    };

    struct NodeProgram {
        std::vector<NodeStatement*> Statements;
    };
}

#endif //QUARTZ_ASTNODE_H
