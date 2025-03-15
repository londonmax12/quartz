#pragma once
#include <unordered_map>
#include <any>

#include <quartz/parser/abstractSyntaxTree.hpp>

namespace Quartz {
	enum VariableType {
		String,
		Float,
		Integer
	};

	struct Variable {
	public:
		Variable() = default;

		Variable(int integer) { type = Integer; data = integer; }
		Variable(float _float) { type = Float; data = _float; }
		Variable(std::string string) { type = String; data = string; }

		VariableType type;

		int getInteger() { return std::any_cast<int>(data); };
		float getFloat() { return std::any_cast<float>(data); };
		std::string getString() { return std::any_cast<std::string>(data); };

	private:
		std::any data;
	};

	class Strategy {
	public:
		std::string name = "";
		std::unique_ptr<StrategyInitNode> initNode = nullptr;
		std::unique_ptr<StrategyOnDataNode> onDataNode = nullptr;

		std::unordered_map<std::string, std::unique_ptr<FunctionDeclNode>> functionNodes;
		std::unordered_map<std::string, Variable> constants;

		Strategy() = default;
	};
}