#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <any>

#include "tokenizer/tokens.hpp"

namespace Quartz {

    enum class NodeType {
        Program,
        ConstDecl,
        Strategy,
        StrategyInitFunction,
        StrategyOnDataFunction,
        FunctionDecl,
        Block,
        ExprStmt,
        Signal,
        IfStmt,
        ReturnStmt,
        CallExpr,
        BinaryExpr,
        IdentifierExpr,
        LiteralExpr,
    };

    enum Signal {
        BUY,
        HOLD,
        SELL
    };

    // Base AST node
    struct ASTNode {
        virtual ~ASTNode() = default;
        virtual void print(int indent = 0) const = 0;
        virtual NodeType nodeType() const = 0;

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
        NodeType nodeType() const override { return NodeType::Program; }
    };

    struct ConstDeclNode : public ASTNode {
        std::string name;
        TokenType type;
        std::string value;
        ConstDeclNode(const std::string& name, TokenType type, const std::string& value)
            : name(name), type(type), value(value) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "ConstDeclNode: " << name << " = " << value << "\n";
        }
        NodeType nodeType() const override { return NodeType::ConstDecl; }
    };

    struct FunctionDeclNode : public ASTNode {
        std::string name;
        TokenType returnType;
        std::unique_ptr<ASTNode> body;
        FunctionDeclNode(const std::string& name, TokenType returnType, std::unique_ptr<ASTNode> body)
            : name(name), returnType(returnType), body(std::move(body)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "FunctionDeclNode: " << name << "\n";
            if (body)
                body->print(indent + 1);
        }
        NodeType nodeType() const override { return NodeType::StrategyOnDataFunction; }
    };

    struct StrategyInitNode : public FunctionDeclNode {
        StrategyInitNode(TokenType returnType, std::unique_ptr<ASTNode> body)
            : FunctionDeclNode("StrategyInit", returnType, std::move(body)) {}

        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "StrategyInitNode: " << name << "\n";
            if (body)
                body->print(indent + 1);
        }

        NodeType nodeType() const override { return NodeType::StrategyInitFunction; }
    };

    struct StrategyOnDataNode : public FunctionDeclNode {
        StrategyOnDataNode(TokenType returnType, std::unique_ptr<ASTNode> body)
            : FunctionDeclNode("StrategyOnData", returnType, std::move(body)) {}

        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "StrategyOnDataNode: " << name << "\n";
            if (body)
                body->print(indent + 1);
        }

        NodeType nodeType() const override { return NodeType::StrategyOnDataFunction; }
    };

    struct StrategyNode : public ASTNode {
        std::string name;
        std::vector<std::unique_ptr<ASTNode>> body;

        std::unique_ptr<StrategyInitNode> initNode = nullptr;
        std::unique_ptr<StrategyOnDataNode> onDataNode = nullptr;

        StrategyNode(const std::string& name) : name(name) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "StrategyNode: " << name << "\n";
            if (initNode) {
                printIndent(indent + 1);
                initNode->print(indent + 1);
            }
            if (onDataNode) {
                printIndent(indent + 1);
                onDataNode->print(indent + 1);
            }
            for (const auto& stmt : body)
                stmt->print(indent + 1);
        }
        NodeType nodeType() const override { return NodeType::Strategy; }
    };

    struct BlockNode : public ASTNode {
        std::vector<std::unique_ptr<ASTNode>> statements;
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "BlockNode\n";
            for (const auto& stmt : statements)
                stmt->print(indent + 1);
        }
        NodeType nodeType() const override { return NodeType::Block; }
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
        NodeType nodeType() const override { return NodeType::ExprStmt; }
    };

    struct SignalNode : public ASTNode {
        Signal signal;
        SignalNode(Signal signal) : signal(signal) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "SignalNode: " << (signal == BUY ? "BUY" : signal == HOLD ? "HOLD" : "SELL") << "\n";
        }
        NodeType nodeType() const override { return NodeType::Signal; }
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
        NodeType nodeType() const override { return NodeType::IfStmt; }
    };

    struct ReturnStmtNode : public ASTNode {
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "ReturnStmtNode\n";
        }
        NodeType nodeType() const override { return NodeType::ReturnStmt; }
    };

    struct CallExprNode : public ASTNode {
        std::string callee;
        std::vector<std::unique_ptr<ASTNode>> arguments;
        CallExprNode(const std::string& callee, std::vector<std::unique_ptr<ASTNode>> arguments)
            : callee(callee), arguments(std::move(arguments)) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "CallExprNode: " << callee << "\n";
            for (const auto& arg : arguments)
                arg->print(indent + 1);
        }
        NodeType nodeType() const override { return NodeType::CallExpr; }
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
        NodeType nodeType() const override { return NodeType::BinaryExpr; }
    };

    struct IdentifierExprNode : public ASTNode {
        std::string name;
        IdentifierExprNode(const std::string& name) : name(name) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "IdentifierExprNode: " << name << "\n";
        }
        NodeType nodeType() const override { return NodeType::IdentifierExpr; }
    };

    struct LiteralExprNode : public ASTNode {
        std::string value;
        LiteralExprNode(const std::string& value) : value(value) {}
        void print(int indent = 0) const override {
            printIndent(indent);
            std::cout << "LiteralExprNode: " << value << "\n";
        }
        NodeType nodeType() const override { return NodeType::LiteralExpr; }
    };
}