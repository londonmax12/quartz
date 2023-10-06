#include "Generator.h"

#include <iostream>

namespace Quartz {
    Generator::Generator(NodeProgram source)
            : m_Source(source), m_Stack(Stack(&m_Out))
    {

    }

    std::string Generator::GenerateProgram() {
        m_Out << "global _start\n";
        m_Out << "_start:\n";

        for (auto& statement : m_Source.Statements) {
            GenerateStatement(statement);
        }

        m_Out << "    mov rax, 60\n";
        m_Out << "    mov rdi, 0\n";
        m_Out << "    syscall\n";

        return m_Out.str();
    }

    void Quartz::Generator::GenerateStatement(NodeStatement* statementNode) {
        auto& statement = statementNode->Statement;

        if (std::holds_alternative<NodeEmpty*>(statement)) {

        }
        else if (std::holds_alternative<NodeStatementExit*>(statement)) {
            GenerateExpr(std::get<NodeStatementExit*>(statement)->Expr);
            m_Out << "    mov rax, 60\n";
            m_Stack.Pop("rdi");
            m_Out << "    syscall\n";
        }
        else if (std::holds_alternative<NodeStatementVarDecl*>(statement)) {
            NodeStatementVarDecl* statementVarDecl = std::get<NodeStatementVarDecl*>(statement);
            std::string identifier = statementVarDecl->Identifier.GetValue();
            if (m_Stack.Contains(identifier)) {
                std::cerr << "Identifier already defined: " << identifier << "\n";
                exit(1);
            }
            m_Stack.Insert(statementVarDecl->Identifier.GetValue());
            GenerateExpr(statementVarDecl->Expr);
        }
        else if (std::holds_alternative<NodeStatementScope*>(statement)) {
            NodeStatementScope* statementScope = std::get<NodeStatementScope*>(statement);
            m_Stack.BeginScope();
            for (auto& stmt : statementScope->Statements) {
                GenerateStatement(stmt);
            }
            m_Stack.EndScope();
        }
    }

    void Generator::GenerateTerm(NodeTerm* term) {
        if (std::holds_alternative<NodeTermIntLit*>(term->Term)) {
            m_Out << "    mov rax, " << std::get<NodeTermIntLit*>(term->Term)->IntLiteral.GetValue() << "\n";
            m_Stack.Push("rax");
        }
        else if (std::holds_alternative<NodeTermIdentifier*>(term->Term)) {
            NodeTermIdentifier* nodeTermIdentifier = std::get<NodeTermIdentifier*>(term->Term);

            Stack::Variable* var = m_Stack.Get(nodeTermIdentifier->Identifier.GetValue());
            if (!var) {
                std::cerr << "Undeclared variable: " << nodeTermIdentifier->Identifier.GetValue() << "\n";
                exit(1);
            }

            std::string offset = "QWORD [rsp + " + std::to_string((m_Stack.Size() - var->StackLocation - 1) * 8) + "]";
            m_Stack.Push(offset);
        }
        else if (std::holds_alternative<NodeTermParen*>(term->Term)) {
            NodeTermParen* nodeTermParen = std::get<NodeTermParen*>(term->Term);
            GenerateExpr(nodeTermParen->Expr);
        }
    }

    void Generator::GenerateBinExpr(NodeBinExpr* expr) {
        auto binExpr = expr->Expr;
        if (std::holds_alternative<NodeBinExprAdd*>(binExpr)) {
            NodeBinExprAdd* nodeAdd = std::get<NodeBinExprAdd*>(binExpr);

            GenerateExpr(nodeAdd->Rhs);
            GenerateExpr(nodeAdd->Lhs);

            m_Stack.Pop("rax");
            m_Stack.Pop("rbx");
            m_Out << "    add rax, rbx\n";
            m_Stack.Push("rax");
        }
        else if (std::holds_alternative<NodeBinExprMulti*>(binExpr)) {
            NodeBinExprMulti* nodeMulti = std::get<NodeBinExprMulti*>(binExpr);

            GenerateExpr(nodeMulti->Rhs);
            GenerateExpr(nodeMulti->Lhs);

            m_Stack.Pop("rax");
            m_Stack.Pop("rbx");
            m_Out << "    imul rbx\n";
            m_Stack.Push("rax");
        }
        else if (std::holds_alternative<NodeBinExprSub*>(binExpr)) {
            NodeBinExprSub* nodeSub = std::get<NodeBinExprSub*>(binExpr);

            GenerateExpr(nodeSub->Rhs);
            GenerateExpr(nodeSub->Lhs);

            m_Stack.Pop("rax");
            m_Stack.Pop("rbx");
            m_Out << "    sub rax, rbx\n";
            m_Stack.Push("rax");
        }
        else if (std::holds_alternative<NodeBinExprDiv*>(binExpr)) {
            NodeBinExprDiv* nodeDiv = std::get<NodeBinExprDiv*>(binExpr);

            GenerateExpr(nodeDiv->Rhs);
            GenerateExpr(nodeDiv->Lhs);

            m_Stack.Pop("rax");
            m_Stack.Pop("rbx");
            m_Out << "    div rbx\n";
            m_Stack.Push("rax");
        }
    }

    void Generator::GenerateExpr(NodeExpr* exprNode) {
        auto expr = exprNode->Expr;
        if (std::holds_alternative<NodeEmpty*>(expr)) {

        }
        else if (std::holds_alternative<NodeBinExpr*>(expr)) {
            NodeBinExpr* nodeBinExpr = std::get<NodeBinExpr*>(expr);
            GenerateBinExpr(nodeBinExpr);
        }
        else if (std::holds_alternative<NodeTerm*>(expr)) {
            GenerateTerm(std::get<NodeTerm*>(expr));
        }
    }
}