#pragma once
#include "abstractSyntaxTree.hpp"
#include "tokenizer/tokens.hpp"

#include <vector>
#include <memory>
#include <string>
#include <any>

namespace Quartz {
    class Parser {
        std::vector<Token> tokens;
        size_t current = 0;

        bool isAtEnd() const { return (current >= tokens.size() || (tokens[current].Type == NONE || tokens[current].Type == END)); }
        Token peek() const { return tokens[current]; }
        Token advance() { if (isAtEnd()) return Token(END); return tokens[current++]; }
        bool match(TokenType type);

        // Program -> (ConstDeclaration | StrategyDeclaration)* ;
        std::unique_ptr<ProgramNode> parseProgram();

        // StrategyDeclaration -> "strategy" IDENTIFIER "{" StrategyBody "}"
        std::unique_ptr<StrategyNode> parseStrategy();

        // ConstDeclaration -> "const" IDENTIFIER (":" Type)? "=" Literal ";" ;
        std::unique_ptr<ConstDeclNode> parseConstDeclaration();

        // FunctionDeclaration -> IDENTIFIER "(" ")" "->" Type Block ;
        std::unique_ptr<FunctionDeclNode> parseFunctionDeclaration();

        // Block -> "{" Statement* "}" ;
        std::unique_ptr<BlockNode> parseBlock();

        // Statement -> IfStatement | ReturnStatement | ExpressionStatement ;
        std::unique_ptr<ASTNode> parseStatement();

        // IfStatement -> "if" "(" Expression ")" Block ( "else" (IfStatement | Block) )? ;
        std::unique_ptr<ASTNode> parseIfStatement();

        // ExpressionStatement -> Expression ";" ;
        std::unique_ptr<ASTNode> parseExpressionStatement();

        // Expression -> BinaryExpression | Primary ;
        std::unique_ptr<ASTNode> parseExpression();

        // Primary -> IDENTIFIER ( "(" ( Expression ("," Expression)* )? ")" )? | Literal ;
        std::unique_ptr<ASTNode> parsePrimary();

    public:
        Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), current(0) {}

        std::shared_ptr<ProgramNode> parse() {
            return parseProgram();
        }
    };
}