#include "Generator.h"

#include <iostream>

std::string Generator::GenerateProgram() {
    m_Out << "global _start\n";
    m_Out << "_start:\n";

    for (const auto& statement : m_Source.Statements) {
        GenerateStatement(statement);
    }

    m_Out << "    mov rax, 60\n";
    m_Out << "    mov rdi, 0\n";
    m_Out << "    syscall\n";

    return m_Out.str();
}

Generator::Generator(NodeProgram source)
    : m_Source(source)
{

}

void Generator::GenerateStatement(const NodeStatement& statement) {

    struct StatementVisitor {
        Generator* generator;
        StatementVisitor(Generator* gen)
            : generator(gen) {};

        void operator()(const NodeEmpty&) {

        }
        void operator()(const NodeStatementExit& statementExit) {
            generator->GenerateExpr(statementExit.Expr);
            generator->m_Out << "    mov rax, 60\n";
            generator->Pop("rdi");
            generator->m_Out << "    syscall\n";
        }
        void operator()(const NodeStatementVarDecl& statementVarDecl) {
            if (generator->m_Variables.find(statementVarDecl.Identifier.GetValue()) != generator->m_Variables.end()) {
                std::cerr << "Identifier already defined: " << statementVarDecl.Identifier.GetValue() << "\n";
                exit(1);
            }
            generator->m_Variables.insert({statementVarDecl.Identifier.GetValue(), Variable(generator->m_StackSize)});
            generator->GenerateExpr(statementVarDecl.Expr);
        }
    };

    StatementVisitor visitor{this};
    std::visit(visitor, statement.Statement);
}

void Generator::GenerateExpr(const NodeExpr &expr) {
    struct ExprVisitor {
        Generator* generator;
        ExprVisitor(Generator* gen)
            : generator(gen) {};

        void operator()(const NodeEmpty&) {

        }
        void operator()(const NodeExprIdentifier& exprIdentifier) {
            if (generator->m_Variables.find(exprIdentifier.Identifier.GetValue()) == generator->m_Variables.end()) {
                std::cerr << "Undeclared variable: " << exprIdentifier.Identifier.GetValue() << "\n";
                exit(1);
            }
            const auto& var = generator->m_Variables.at(exprIdentifier.Identifier.GetValue());
            std::stringstream offset;
            offset << "QWORD [rsp + " << (generator->m_StackSize - var.StackLocation - 1) * 8 << "]\n";
            generator->Push(offset.str());
        }
        void operator()(const NodeExprIntLit& exprIntLit) {
            generator->m_Out << "    mov rax, " << exprIntLit.IntLiteral.GetValue() << "\n";
            generator->Push("rax");
        }
    };

    ExprVisitor visitor{this};
    std::visit(visitor, expr.Expr);
}

void Generator::Push(const std::string &reg) {
    m_Out << "    push " << reg << "\n";
    m_StackSize++;
}

void Generator::Pop(const std::string &reg) {
    m_Out << "    pop " << reg << "\n";
    m_StackSize--;
}
