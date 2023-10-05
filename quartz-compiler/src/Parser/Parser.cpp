#include "Parser.h"

#include <iostream>

namespace Quartz {
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
        while (Peak().IsValid())
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
        TokenType term =  Peak().GetType();
        switch (term) {
            case TokenType::INT_LIT: {
                nodeTerm = m_Pool.Allocate<NodeTerm>();
                NodeTermIntLit* nodeTermIntLit = m_Pool.Allocate<NodeTermIntLit>();
                Token token = Consume();
                nodeTermIntLit->IntLiteral = token;
                nodeTerm->Term = nodeTermIntLit;
                break;
            }
            case TokenType::IDENTIFIER: {
                nodeTerm = m_Pool.Allocate<NodeTerm>();
                NodeTermIdentifier* nodeTermIdentifier = m_Pool.Allocate<NodeTermIdentifier>();
                Token token = Consume();
                nodeTermIdentifier->Identifier = token;
                nodeTerm->Term = nodeTermIdentifier;
                break;
            }
            case TokenType::OPEN_PAREN: {
                Token token = Consume();
                NodeExpr* nodeExpr = ParseExpr();
                nodeTerm = m_Pool.Allocate<NodeTerm>();

                if (!nodeExpr) {
                    std::cerr << "Expected expression\n";
                    exit(1);
                }

                if (Peak().GetType() != TokenType::CLOSE_PAREN) {
                    std::cerr << "Expected \')\'\n";
                    exit(1);
                }

                Consume();

                NodeTermParen* nodeTermParen = m_Pool.Allocate<NodeTermParen>();
                nodeTermParen->Expr = nodeExpr;
                nodeTerm->Term = nodeTermParen;
                break;
            }
            default:
                break;
        }

        return nodeTerm;
    }

    NodeExpr* Parser::ParseExpr(int minPrec) {
        NodeExpr* nodeExpr = nullptr;
        NodeTerm* nodeTermLhs = ParseTerm();

        if (!nodeTermLhs)
            return nodeExpr;

        NodeExpr* nodeExprLhs = m_Pool.Allocate<NodeExpr>();
        nodeExprLhs->Expr = nodeTermLhs;

        while (true) {
            Token currToken = Peak();
            int prec = 0;

            if (!currToken.IsValid() || !currToken.IsBinOperator()) {
                prec = currToken.GetBinPrec();
                if (prec == -1 || prec < minPrec)
                    break;
            } else {
                prec = currToken.GetBinPrec();
                if (prec < minPrec)
                    break;
            }

            Token token = Consume();
            int nextMinPrec = prec + 1;
            NodeExpr* nodeExprRhs = ParseExpr(nextMinPrec);

            if (!nodeExprRhs) {
                std::cerr << "Failed to parse expression\n";
            }

            NodeBinExpr* binExpr = m_Pool.Allocate<NodeBinExpr>();
            NodeExpr* nodeExprLhs2 = m_Pool.Allocate<NodeExpr>();
            switch (token.GetType()) {
                case TokenType::PLUS: {
                    NodeBinExprAdd* nodeBinAdd = m_Pool.Allocate<NodeBinExprAdd>();
                    nodeExprLhs2->Expr = nodeExprLhs->Expr;
                    nodeBinAdd->Lhs = nodeExprLhs2;
                    nodeBinAdd->Rhs = nodeExprRhs;
                    binExpr->Expr = nodeBinAdd;
                    break;
                }
                case TokenType::MULTI: {
                    NodeBinExprMulti* nodeBinMulti = m_Pool.Allocate<NodeBinExprMulti>();
                    nodeExprLhs2->Expr = nodeExprLhs->Expr;
                    nodeBinMulti->Lhs = nodeExprLhs2;
                    nodeBinMulti->Rhs = nodeExprRhs;
                    binExpr->Expr = nodeBinMulti;
                    break;
                }
                case TokenType::DIV: {
                    NodeBinExprDiv* nodeBinDiv = m_Pool.Allocate<NodeBinExprDiv>();
                    nodeExprLhs2->Expr = nodeExprLhs->Expr;
                    nodeBinDiv->Lhs = nodeExprLhs2;
                    nodeBinDiv->Rhs = nodeExprRhs;
                    binExpr->Expr = nodeBinDiv;
                    break;
                }
                case TokenType::SUB: {
                    NodeBinExprSub* nodeBinSub = m_Pool.Allocate<NodeBinExprSub>();
                    nodeExprLhs2->Expr = nodeExprLhs->Expr;
                    nodeBinSub->Lhs = nodeExprLhs2;
                    nodeBinSub->Rhs = nodeExprRhs;
                    binExpr->Expr = nodeBinSub;
                    break;
                }
            }

            nodeExprLhs->Expr = binExpr;
        }

        return nodeExprLhs;
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
}