#include "Parser.h"

#include "../Logging/Logging.h"

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
        try {
            NodeProgram nodeProgram;
            while (Peak().IsValid()) {
                NodeStatement* statement = ParseStatement();
                if (!statement)
                    throw(ParserException("Invalid statement while parsing program"));

                nodeProgram.Statements.push_back(statement);
            }
            return nodeProgram;
        } catch (const MemoryAllocationException& e) {
            throw ParserException(e.what());
        }
    }

    NodeTerm* Parser::ParseTerm() {
        NodeTerm* nodeTerm = nullptr;
        TokenType term =  Peak().GetType();
        switch (term) {
            case TokenType::INT_LIT: {
                Token token = Consume();
                NodeTermIntLit* nodeTermIntLit = m_Pool.Allocate<NodeTermIntLit>(token);
                nodeTerm = m_Pool.Allocate<NodeTerm>(nodeTermIntLit);
                break;
            }
            case TokenType::IDENTIFIER: {
                Token token = Consume();
                NodeTermIdentifier* nodeTermIdentifier = m_Pool.Allocate<NodeTermIdentifier>(token);
                nodeTerm = m_Pool.Allocate<NodeTerm>(nodeTermIdentifier);
                break;
            }
            case TokenType::OPEN_PAREN: {
                Token token = Consume();

                NodeExpr* nodeExpr = ParseExpr();
                if (!nodeExpr)
                    throw(ParserException("Expected expression", token));

                TryConsume(TokenType::CLOSE_PAREN, "Expected \')\'");

                NodeTermParen* nodeTermParen = m_Pool.Allocate<NodeTermParen>(nodeExpr);
                nodeTerm = m_Pool.Allocate<NodeTerm>(nodeTermParen);
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

        NodeExpr* nodeExprLhs = m_Pool.Allocate<NodeExpr>(nodeTermLhs);

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

            if (!nodeExprRhs)
                throw(ParserException("Failed to parse expression"));

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

    NodeScope* Parser::ParseScope() {
        if (Peak().GetType() != TokenType::OPEN_CURLY)
            return nullptr;
        Consume();

        NodeScope* scope = m_Pool.Allocate<NodeScope>();
        NodeStatement* nodeStatement = ParseStatement();
        while (nodeStatement) {
            scope->Statements.push_back(nodeStatement);
            nodeStatement = ParseStatement();
        }

        TryConsume(TokenType::CLOSE_CURLY,  "Expected \'}\'");

        return scope;
    }

    NodeStatement* Parser::ParseStatement() {
        switch (Peak().GetType()) {
            case TokenType::EXIT: {
                Consume();
                TryConsume(TokenType::OPEN_PAREN, "Expected \'(\'");
                Token t = Peak();
                NodeExpr* nodeExpr = ParseExpr();
                if (!nodeExpr) {
                    throw(ParserException("Expected expression", t));
                }

                NodeStatementExit* nodeExit = m_Pool.Allocate<NodeStatementExit>(nodeExpr);

                TryConsume(TokenType::CLOSE_PAREN, "Expected \')\'");
                TryConsume(TokenType::ENDL, "Expected \';\'");

                NodeStatement* statement = m_Pool.Allocate<NodeStatement>(nodeExit);
                return statement;
            }
            case TokenType::VAR: {
                Consume();
                Token varIdentifier = TryConsume(TokenType::IDENTIFIER, "Expected identifier");
                TryConsume(TokenType::COLON, "Expected \':\'");
                TryConsume(TokenType::VAR_INT, "Expected variable type");
                TryConsume(TokenType::EQUALS, "Expected \'=\'");\

                Token t = Peak();
                NodeExpr* varExpr = ParseExpr();

                if (std::holds_alternative<NodeEmpty*>(varExpr->Expr))
                    throw(ParserException("Expected expression", t));

                NodeStatementVarDecl* varDecl = m_Pool.Allocate<NodeStatementVarDecl>(varIdentifier, varExpr);

                if (Peak().GetType() != TokenType::ENDL)
                    throw(ParserException("Expected \';\'"));

                Consume();

                NodeStatement* statement = m_Pool.Allocate<NodeStatement>();
                statement->Statement = varDecl;
                return statement;
            }
            case TokenType::OPEN_CURLY: {
                NodeScope* scope = ParseScope();
                if (!scope)
                    throw(ParserException("Invalid scope"));

                NodeStatement* statement = m_Pool.Allocate<NodeStatement>(scope);
                return statement;
            }
            case TokenType::IF: {
                Consume();

                if (Peak().GetType() != TokenType::OPEN_PAREN)
                    throw(ParserException("Expected \'(\'"));

                Consume();

                Token t = Peak();
                NodeExpr* nodeExpr = ParseExpr();
                if (!nodeExpr)
                    throw(ParserException("Expected expression", t));

                if (Peak().GetType() != TokenType::CLOSE_PAREN)
                    throw(ParserException("Expected \')\'"));

                Consume();

                NodeStatement* nodeStatement = ParseStatement();
                if (!nodeStatement)
                    throw(ParserException("Failed to parse statement"));

                NodeStatementIf* nodeStatementIf = m_Pool.Allocate<NodeStatementIf>(nodeExpr, nodeStatement);
                NodeStatement* statement = m_Pool.Allocate<NodeStatement>(nodeStatementIf);
                return statement;
            }
            default: {
                return nullptr;
            }
        }
    }

    Token Parser::TryConsume(TokenType type, const std::string& errMessage) {
        if (Peak().GetType() != type)
            throw(ParserException(errMessage));

        return Consume();
    }
}