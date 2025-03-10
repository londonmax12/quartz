#include "quartz.hpp"

namespace Quartz {
    void run_file(const char* filepath)
    {
		Logger::getInstance().logf(Logger::INFO, "Reading file content: %s", filepath);
		const char* fileContents = Quartz::loadFileToCString(filepath);

		Logger::getInstance().log(Logger::INFO, "Tokenizing");
		Quartz::Tokenizer tokenizer = Quartz::Tokenizer();
		const std::vector<Quartz::Token> tokens = tokenizer.tokenize(fileContents);

		printf("\n");
		for (auto& token : tokens) {
			std::cout << std::string(token) << std::endl;
		}
    }
}