#pragma once
#include <string>
#include <vector>
#include <memory>
#include <any>
#include <iostream>

#include "tokenizer/tokens.hpp"

namespace Quartz {
    enum Signal {
        BUY,
        HOLD,
        SELL
    };

    // Base AST node
    struct ASTNode {
        virtual ~ASTNode() = default;
        virtual void print(int indent = 0) const = 0;
    protected:
        void printIndent(int indent) const {
            for (int i = 0; i < indent; ++i)
                std::cout << "  ";
        }
    };

    struct ProgramNode : public ASTNode {
        std::vector<std::unique_ptr<ASTNode>> declarations;
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "ProgramNode\n";
            for (const auto& decl : declarations)
                decl->print(indent + 1);
        }
    };

    struct ConstDeclNode : public ASTNode {
        std::string name;
        TokenType type;
        std::string value;
        ConstDeclNode(const std::string &name, TokenType type, const std::string &value)
            : name(name), type(type), value(value) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "ConstDeclNode: " << name << " = " << value << "\n";
        }
    };

    struct StrategyNode : public ASTNode {
        std::string name;
        std::vector<std::unique_ptr<ASTNode>> body;
        StrategyNode(const std::string &name) : name(name) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "StrategyNode: " << name << "\n";
            for (const auto& stmt : body)
                stmt->print(indent + 1);
        }
    };

    struct FunctionDeclNode : public ASTNode {
        std::string name;
        TokenType returnType;
        std::unique_ptr<ASTNode> body;
        FunctionDeclNode(const std::string &name, TokenType returnType, std::unique_ptr<ASTNode> body)
            : name(name), returnType(returnType), body(std::move(body)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "FunctionDeclNode: " << name << "\n";
            if (body)
                body->print(indent + 1);
        }
    };

    struct BlockNode : public ASTNode {
        std::vector<std::unique_ptr<ASTNode>> statements;
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "BlockNode\n";
            for (const auto& stmt : statements)
                stmt->print(indent + 1);
        }
    };

    struct ExprStmtNode : public ASTNode {
        std::unique_ptr<ASTNode> expression;
        ExprStmtNode(std::unique_ptr<ASTNode> expression)
            : expression(std::move(expression)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "ExprStmtNode\n";
            expression->print(indent + 1);
        }
    };

    struct SignalNode : public ASTNode {
        Signal signal;
        SignalNode(Signal signal) : signal(signal) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "SignalNode: " << (signal == BUY ? "BUY" : signal == HOLD ? "HOLD" : "SELL") << "\n";
        }
    };

    struct IfStmtNode : public ASTNode {
        std::unique_ptr<ASTNode> condition;
        std::unique_ptr<BlockNode> thenBlock;
        std::unique_ptr<ASTNode> elseBranch;
        IfStmtNode(std::unique_ptr<ASTNode> condition,
                   std::unique_ptr<BlockNode> thenBlock,
                   std::unique_ptr<ASTNode> elseBranch)
            : condition(std::move(condition)),
              thenBlock(std::move(thenBlock)),
              elseBranch(std::move(elseBranch)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "IfStmtNode\n";
            condition->print(indent + 1);
            thenBlock->print(indent + 1);
            if (elseBranch) {
                printIndent(indent);
                std::cout << "ElseBranch:\n";
                elseBranch->print(indent + 1);
            }
        }
    };

    struct ReturnStmtNode : public ASTNode {
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "ReturnStmtNode\n";
        }
    };

    struct CallExprNode : public ASTNode {
        std::string callee;
        std::vector<std::unique_ptr<ASTNode>> arguments;
        CallExprNode(const std::string &callee, std::vector<std::unique_ptr<ASTNode>> arguments)
            : callee(callee), arguments(std::move(arguments)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "CallExprNode: " << callee << "\n";
            for (const auto& arg : arguments)
                arg->print(indent + 1);
        }
    };

    struct BinaryExprNode : public ASTNode {
        std::unique_ptr<ASTNode> left;
        Token op;
        std::unique_ptr<ASTNode> right;
        BinaryExprNode(std::unique_ptr<ASTNode> left, Token op, std::unique_ptr<ASTNode> right)
            : left(std::move(left)), op(op), right(std::move(right)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "BinaryExprNode: " << std::string(op) << "\n";
            left->print(indent + 1);
            right->print(indent + 1);
        }
    };

    struct IdentifierExprNode : public ASTNode {
        std::string name;
        IdentifierExprNode(const std::string &name) : name(name) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "IdentifierExprNode: " << name << "\n";
        }
    };

    struct LiteralExprNode : public ASTNode {
        std::string value;
        LiteralExprNode(const std::string &value) : value(value) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "LiteralExprNode: " << value << "\n";
        }
    };
}
