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
    class Generator {
    public:
        Generator(NodeProgram source);

        std::string GenerateProgram();
    private:
        void GenerateExpr(NodeExpr* expr);
        void GenerateBinExpr(NodeBinExpr* expr);
        void GenerateStatement(NodeStatement* statement);
        void GenerateTerm(NodeTerm* term);

        NodeProgram m_Source;
        std::stringstream m_Out;

        Stack m_Stack;
    };
}

#endif
