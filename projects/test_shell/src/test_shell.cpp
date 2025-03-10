#include "quartz/quartz.hpp"
#include <direct.h>

int main() {
	const char* fileContents = Quartz::loadFileToCString("moving_average_crossover.qz");
	Quartz::Tokenizer tokenizer = Quartz::Tokenizer();
	const std::vector<Quartz::Token> tokens = tokenizer.tokenize(fileContents);
	printf("\n");
	for (auto& token : tokens) {
		std::cout << std::string(token) << std::endl;
	}
}