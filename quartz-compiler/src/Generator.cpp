#include "Generator.h"

#include <iostream>
#include <sstream>

std::string Generator::GenerateASM() {
    std::stringstream out;

    out << "global _start\n";
    out << "_start:\n";
    out << "    mov rax, 60\n";
    out << "    mov rdi, " << m_Source.Expr.IntLiteral.GetValue() << "\n";
    out << "    syscall";

    return out.str();
}

Generator::Generator(ExitNode source)
    : m_Source(source)
{

}
