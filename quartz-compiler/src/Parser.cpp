#include "Parser.h"

#include <iostream>

Parser::Parser(std::vector<Token> tokens)
    : m_Tokens(tokens), m_Pool(1024 * 1024 * 4) {
    m_Pool.Init();
}

Parser::~Parser() {
    m_Pool.Free();
}

Token Parser::Peak(int offset) {
    if (m_Index + offset >= m_Tokens.size()) {
        return {TokenType::NONE};
    }
    return m_Tokens[m_Index + offset];
}

Token Parser::Consume(int amount) {
    int index = 0;
    for(int i = 0; i < amount; i++) {
        index = m_Index++;
    }
    return m_Tokens[index];
}

NodeProgram Parser::ParseProgram() {
    NodeProgram nodeProgram;
    while (Peak().GetType() != TokenType::NONE)
    {
        NodeStatement statement = ParseStatement();
        if (std::holds_alternative<NodeEmpty*>(statement.Statement)) {
            std::cerr << "Invalid statement\n";
            exit(1);
        }
        nodeProgram.Statements.push_back(statement);
    }
    return nodeProgram;
}

NodeTerm* Parser::ParseTerm() {
    NodeTerm* nodeTerm = nullptr;

    if (Peak().GetType() == TokenType::INT_LIT) {
        nodeTerm = m_Pool.Allocate<NodeTerm>();
        NodeTermIntLit* nodeTermIntLit = m_Pool.Allocate<NodeTermIntLit>();
        Token token = Consume();
        nodeTermIntLit->IntLiteral = token;
        nodeTerm->Term = nodeTermIntLit;
    }
    else if (Peak().GetType() == TokenType::IDENTIFIER) {
        nodeTerm = m_Pool.Allocate<NodeTerm>();
        NodeTermIdentifier* nodeTermIdentifier = m_Pool.Allocate<NodeTermIdentifier>();
        Token token = Consume();
        nodeTermIdentifier->Identifier = token;
        nodeTerm->Term = nodeTermIdentifier;
    }

    return nodeTerm;
}

NodeExpr* Parser::ParseExpr() {
    NodeExpr* nodeExpr = nullptr;

    NodeTerm* nodeTerm = ParseTerm();
    if (nodeTerm) {
        nodeExpr = m_Pool.Allocate<NodeExpr>();
        if (Peak().GetType() == TokenType::PLUS) {
            NodeBinExpr* nodeBinExpr = m_Pool.Allocate<NodeBinExpr>();
            NodeBinExprAdd* nodeBinExprAdd =  m_Pool.Allocate<NodeBinExprAdd>();
            NodeExpr* nodeExprLhs = m_Pool.Allocate<NodeExpr>();
            nodeExprLhs->Expr = nodeTerm;
            nodeBinExprAdd->Lhs = nodeExprLhs;

            Consume();
            NodeExpr* nodeExprRhs = ParseExpr();
            if (!nodeExprRhs) {
                std::cerr << "Expected expression\n";
                exit(1);
            }
            nodeBinExprAdd->Rhs = nodeExprRhs;
            nodeBinExpr->Expr = nodeBinExprAdd;

            nodeExpr->Expr = nodeBinExpr;
        }
        else {
            nodeExpr->Expr = nodeTerm;
        }
    }

    return nodeExpr;
}

NodeStatement Parser::ParseStatement() {
    switch (Peak().GetType()) {
        case TokenType::EXIT: {
            if (Peak(1).GetType() != TokenType::OPEN_PAREN) {
                std::cerr << "Expected \'(\'\n";
                exit(1);
            }
            Consume(2);

            NodeExpr* nodeExpr = ParseExpr();
            if (std::holds_alternative<NodeEmpty*>(nodeExpr->Expr)) {
                std::cerr << "Invalid expression\n";
                exit(1);
            }

            NodeStatementExit* nodeExit = m_Pool.Allocate<NodeStatementExit>();
            nodeExit->Expr = nodeExpr;

            if (Peak().GetType() != TokenType::CLOSE_PAREN) {
                std::cerr << "Expected \')\'\n";
                exit(1);
            }
            Consume();

            if (Peak().GetType() != TokenType::ENDL) {
                std::cerr << "Expected \';\'\n";
                exit(1);
            }
            Consume();

            return NodeStatement{nodeExit};
        }
        case TokenType::VAR: {
            Consume();

            if (Peak().GetType() != TokenType::IDENTIFIER) {
                std::cerr << "Expected identifier\n";
                exit(1);
            }
            if (Peak(1).GetType() != TokenType::COLON) {
                std::cerr << "Expected \':\'\n";
                exit(1);
            }
            if (Peak(2).GetType() != TokenType::VAR_INT) {
                std::cerr << "Expected variable type\n";
                exit(1);
            }
            if (Peak(3).GetType() != TokenType::EQUALS) {
                std::cerr << "Expected \'=\'\n";
                exit(1);
            }

            Token varIdentifier = Consume();
            Consume(3);

            NodeExpr* varExpr = ParseExpr();

            if (std::holds_alternative<NodeEmpty*>(varExpr->Expr)) {
                std::cerr << "Expected expression\n";
                exit(1);
            }

            NodeStatementVarDecl* varDecl = m_Pool.Allocate<NodeStatementVarDecl>();
            varDecl->Identifier = varIdentifier;
            varDecl->Expr = varExpr;

            if (Peak().GetType() != TokenType::ENDL) {
                std::cerr << "Expected \';\'\n";
                exit(1);
            }
            Consume();

            return NodeStatement{varDecl};
        }
        default: {
            Consume();
            return NodeStatement{(NodeEmpty*)nullptr};
        }
    }
}