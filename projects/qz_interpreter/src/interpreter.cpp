#include "interpreter.hpp"

#include <typeinfo>

Quartz::Variable Quartz::Interpreter::parseConstant(const ConstDeclNode* node)
{
	Variable constant = Variable(node->value);

	return constant;
}

std::unique_ptr<Quartz::Strategy> Quartz::Interpreter::parseStrategy(std::unique_ptr<Quartz::ASTNode> node)
{
	StrategyNode* strategyNode = dynamic_cast<StrategyNode*>(node.get());
	std::unique_ptr<Strategy> strategy = std::make_unique<Strategy>();

	strategy->name = strategyNode->name;
	strategy->initNode = std::move(strategyNode->initNode);
	strategy->onDataNode = std::move(strategyNode->onDataNode);

	for (auto& node : strategyNode->body) {
		switch (node->nodeType())
		{
		case NodeType::ConstDecl: {
			ConstDeclNode* constNode = dynamic_cast<ConstDeclNode*>(node.get());
			strategy->constants[constNode->name] = std::move(parseConstant(constNode));
			break;
		}
		default:
			break;
		}
	}

	return strategy;
}

void Quartz::Interpreter::interpret()
{
	mProgramNode->print();

	auto& statements = mProgramNode->declarations;
	for (auto& statement : statements) {
		NodeType type = statement->nodeType();
		switch (type)
		{
		case NodeType::Strategy:
		{
			parseStrategy(std::move(statement));
			break;
		}
		default:
			break;
		}
	}
}
