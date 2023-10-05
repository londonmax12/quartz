#include "Generator.h"

#include <iostream>

std::string Generator::GenerateProgram() {
    m_Out << "global _start\n";
    m_Out << "_start:\n";

    for (auto& statement : m_Source.Statements) {
        GenerateStatement(&statement);
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
void Generator::GenerateStatement(NodeStatement* statementNode) {
    auto& statement = statementNode->Statement;

    if (std::holds_alternative<NodeEmpty*>(statement)) {

    }
    else if (std::holds_alternative<NodeStatementExit*>(statement)) {
        GenerateExpr(std::get<NodeStatementExit*>(statement)->Expr);
        m_Out << "    mov rax, 60\n";
        Pop("rdi");
        m_Out << "    syscall\n";
    }
    else if (std::holds_alternative<NodeStatementVarDecl*>(statement)) {
        NodeStatementVarDecl* statementVarDecl = std::get<NodeStatementVarDecl*>(statement);
        std::string identifier = statementVarDecl->Identifier.GetValue();
        if (m_Variables.find(identifier) != m_Variables.end()) {
            std::cerr << "Identifier already defined: " << statementVarDecl->Identifier.GetValue() << "\n";
            exit(1);
        }
        m_Variables.insert({statementVarDecl->Identifier.GetValue(), Variable(m_StackSize)});
        GenerateExpr(statementVarDecl->Expr);
    }
}

void Generator::GenerateTerm(NodeTerm* term) {
    if (std::holds_alternative<NodeTermIntLit*>(term->Term)) {
        m_Out << "    mov rax, " << std::get<NodeTermIntLit*>(term->Term)->IntLiteral.GetValue() << "\n";
        Push("rax");
    }
    else if (std::holds_alternative<NodeTermIdentifier*>(term->Term)) {
        NodeTermIdentifier* nodeTermIdentifier = std::get<NodeTermIdentifier*>(term->Term);
        if (m_Variables.find(nodeTermIdentifier->Identifier.GetValue()) == m_Variables.end()) {
            std::cerr << "Undeclared variable: " << nodeTermIdentifier->Identifier.GetValue() << "\n";
            exit(1);
        }
        const auto& var = m_Variables.at(nodeTermIdentifier->Identifier.GetValue());
        std::stringstream offset;
        offset << "QWORD [rsp + " << (m_StackSize - var.StackLocation - 1) * 8 << "]\n";
        Push(offset.str());
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

        Pop("rax");
        Pop("rbx");
        m_Out << "    add rax, rbx\n";
        Push("rax");
    }
    else if (std::holds_alternative<NodeBinExprMulti*>(binExpr)) {
        NodeBinExprMulti* nodeMulti = std::get<NodeBinExprMulti*>(binExpr);

        GenerateExpr(nodeMulti->Rhs);
        GenerateExpr(nodeMulti->Lhs);

        Pop("rax");
        Pop("rbx");
        m_Out << "    imul rbx\n";
        Push("rax");
    }
    else if (std::holds_alternative<NodeBinExprSub*>(binExpr)) {
        NodeBinExprSub* nodeSub = std::get<NodeBinExprSub*>(binExpr);

        GenerateExpr(nodeSub->Rhs);
        GenerateExpr(nodeSub->Lhs);

        Pop("rax");
        Pop("rbx");
        m_Out << "    sub rax, rbx\n";
        Push("rax");
    }
    else if (std::holds_alternative<NodeBinExprDiv*>(binExpr)) {
        NodeBinExprDiv* nodeDiv = std::get<NodeBinExprDiv*>(binExpr);

        GenerateExpr(nodeDiv->Rhs);
        GenerateExpr(nodeDiv->Lhs);

        Pop("rax");
        Pop("rbx");
        m_Out << "    div rbx\n";
        Push("rax");
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

void Generator::Push(const std::string &reg) {
    m_Out << "    push " << reg << "\n";
    m_StackSize++;
}

void Generator::Pop(const std::string &reg) {
    m_Out << "    pop " << reg << "\n";
    m_StackSize--;
}