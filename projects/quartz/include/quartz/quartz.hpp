#include "../pch.hpp"

#include "parser/abstractSyntaxTree.hpp"

namespace Quartz {
	std::shared_ptr<ProgramNode> run_code(const char* code);
	std::shared_ptr<ProgramNode> run_file(const char* file);
}