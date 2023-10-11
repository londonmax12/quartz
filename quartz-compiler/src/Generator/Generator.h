#ifndef QUARTZ_GENERATOR_H
#define QUARTZ_GENERATOR_H

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "../Lexer/Token.h"
#include "../Parser/AstNode.h"
#include "Stack.h"

namespace Quartz {
    class GeneratorException : public std::exception {
    public:
        GeneratorException(const std::string& message)
            : m_ErrorMessage(message) {}

        const char* what() const noexcept override {
            return m_ErrorMessage.c_str();
        }
    private:
        std::string m_ErrorMessage;
    };

    class Generator {
    public:
        Generator(NodeProgram source);

        std::string GenerateProgram();
    private:
        void GenerateExpr(NodeExpr* expr);
        void GenerateBinExpr(NodeBinExpr* binExpr);
        void GenerateScope(NodeScope* scope);
        void GenerateStatement(NodeStatement* statement);
        void GenerateTerm(NodeTerm* term);

        NodeProgram m_Source;
        std::stringstream m_Out;

        Stack m_Stack;
        size_t m_LabelCount;
    };
}

#endif
