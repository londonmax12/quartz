#pragma once

#include "pch.hpp"

#include "tokenizer/tokens.hpp"

namespace Quartz {
	class Tokenizer {
	private:
		char peek(const char* input, int i);
		Token buildNumber(const char* input, int* i);

		Token parseString(const std::string& string);
	public:
		std::vector<Token> tokenize(const char* input);
	};
}