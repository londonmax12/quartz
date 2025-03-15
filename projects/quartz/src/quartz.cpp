#include "quartz.hpp"

#include "utils/fileUtils.hpp"
#include "logging/logging.hpp"
#include "tokenizer/tokenizer.hpp"
#include "parser/parser.hpp"
#include "parser/abstractSyntaxTree.hpp"

namespace Quartz {
	std::shared_ptr<ProgramNode> run_code(const char* code)
	{
		Logger::getInstance().log(Logger::INFO, "Tokenizing");
		Tokenizer tokenizer = Tokenizer(code);
		const std::vector<Token> tokens = tokenizer.tokenize();

		Logger::getInstance().log(Logger::INFO, "Parsing tokens");
		Parser parser = Parser(tokens);
		auto& programNode = parser.parse();

		return programNode;
	}

	std::shared_ptr<ProgramNode> run_file(const char* filepath)
    {
		Logger::getInstance().logf(Logger::INFO, "Reading file content: %s", filepath);
		const char* fileContents = loadFileToCString(filepath);


		return run_code(fileContents);
    }
}