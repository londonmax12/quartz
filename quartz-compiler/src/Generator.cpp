#include "Generator.h"

#include <iostream>
#include <sstream>

std::string Generator::GenerateASM(const std::vector<Token> &tokens) {
    std::stringstream out;

    out << "global _start\n";
    out << "_start:\n";

    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.m_Type == TokenType::RETURN) {
            if (i + 2 < tokens.size()) {
                Token exitCode = tokens.at(i + 1);
                Token endline = tokens.at(i + 2);

                if (exitCode.m_Type != TokenType::INT_LIT) {
                    std::cerr << "Expected exit code\n";
                }
                if (endline.m_Type != TokenType::ENDL) {
                    std::cerr << "Expected line break\n";
                }

                out << "    mov rax, 60\n";
                out << "    mov rdi, " << exitCode.m_Value << "\n";
                out << "    syscall";
            }
        }
    }

    return out.str();
}
