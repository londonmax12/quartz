#pragma once

#include "pch.hpp"

#include "tokenizer/tokens.hpp"
#include "logging/logging.hpp"

namespace Quartz {
	class Tokenizer {
	private:
		const char* mInput = "";

		char peek(int i);
		Token buildNumber(int* i);

		Token parseString(const std::string& string);
	public:
		std::vector<Token> tokenize();

		Tokenizer(const char* mInput)
			: mInput(mInput) {};
	};
}