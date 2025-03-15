#pragma once

#include <memory>

#include <quartz/parser/abstractSyntaxTree.hpp>

#include "strategy/strategy.hpp"

namespace Quartz {
	class Interpreter {
	private:
		std::shared_ptr<ProgramNode> mProgramNode;
		Variable parseConstant(const ConstDeclNode* node);
		std::unique_ptr<Strategy> parseStrategy(const std::unique_ptr<ASTNode> node);
	public:
		Interpreter(std::shared_ptr<ProgramNode> programNode)
			: mProgramNode(programNode) {};

		void interpret();
	};
}