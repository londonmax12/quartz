#include "tokenizer/tokenizer.hpp"

namespace Quartz {
	char Tokenizer::peek(int i)
	{
		return mInput[i + 1] != '\0' ? mInput[i + 1] : '\0';
	}

	Token Tokenizer::buildNumber(int* index)
	{
		std::string number = "";
		bool isFloat = false;
		for (int i = *index; mInput[i] != '\0'; ++i) {
			(*index)++;
			char currChar = mInput[i];
			if (std::isdigit(currChar)) {
				number += currChar;
			}
			else if (currChar == '.') {
				if (isFloat) {
					std::cerr << "Unexpected symbol: " << currChar << std::endl;
				}
				isFloat = true;
			}
			else {
				return Token(isFloat ? FLOAT_VALUE : INT_VALUE, number);
			}
		}
		return Token(isFloat ? FLOAT_VALUE : INT_VALUE, number);
	}

	Token Tokenizer::parseString(const std::string& string)
	{
		Token token;
		auto position = keywordMap.find(string);
		if (position != keywordMap.end()) {
			token = Token(position->second);
		}
		else {
			token = Token(TokenType::IDENTIFIER, string);
		}
		return token;
	}

	std::vector<Token> Tokenizer::tokenize() {
		std::vector<Token> tokens = {};
		std::string currString = "";
		bool isSingleLineComment = false;
		char inString = 'n';

		int line = 1;
		int charPos = 0;

		for (int i = 0; mInput[i] != '\0'; ++i) {
			charPos++;
			char currChar = mInput[i];
			switch (currChar)
			{
			case ' ':
			case '\t':
				if (isSingleLineComment)
					break;

				if (!currString.empty())
				{
					tokens.push_back(parseString(currString));
					currString = "";
				}
				break;
			case '\"':
				if (isSingleLineComment)
					break;

				if (inString == '\'') {
					currString += currChar;
					break;
				}
				else if (inString == '\"') {
					inString = 'n';
					if (!currString.empty())
					{
						tokens.push_back(Token(STRING_VALUE, currString));
						currString = "";
					}
					break;
				}
				inString = '\"';
				break;
			case '\'':
				if (isSingleLineComment)
					break;

				if (inString == '\"') {
					currString += currChar;
					break;
				}
				else if (inString == '\'') {
					inString = 'n';
					if (!currString.empty())
					{
						tokens.push_back(Token(STRING_VALUE, currString));
						currString = "";
					}
					break;
				}
				inString = '\'';
				break;
			case '{':
				if (isSingleLineComment)
					break;

				if (!currString.empty())
				{
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(OPEN_CURLY_BRACE));
				break;
			case '}':
				if (isSingleLineComment)
					break;

				if (!currString.empty())
				{
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(CLOSE_CURLY_BRACE));
				break;
			case '/':
				if (isSingleLineComment)
					break;

				if (peek(i) == '/') {
					isSingleLineComment = true;
					i++;
				}
				if (!currString.empty())
				{
					tokens.push_back(parseString(currString));
					currString = "";
				}
				break;
			case '=':
				if (isSingleLineComment)
					break;

				if (!currString.empty())
				{
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(EQUALS));
				break;
			case ':':
				if (isSingleLineComment)
					break;

				if (!currString.empty())
				{
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(COLON));
				break;
			case ',':
				if (isSingleLineComment)
					break;

				if (!currString.empty()) {
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(COMMA));
				break;
			case '(':
				if (isSingleLineComment)
					break;

				if (!currString.empty()) {
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(OPEN_BRACKET));
				break;
			case ')':
				if (isSingleLineComment)
					break;

				if (!currString.empty()) {
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(CLOSE_BRACKET));
				break;
			case '-':
				if (isSingleLineComment)
					break;

				if (peek(i) == '>') {
					tokens.push_back(Token(RIGHT_ARROW));
					i++;
				}
				if (!currString.empty()) {
					tokens.push_back(parseString(currString));
					currString = "";
				}
				break;
			case '\n':
			case '\r':
				if (currChar == '\n' && peek(i) == '\r') {
					i++;
				} else if (currChar == '\r' && peek(i) == '\n') {
					i++;
				}

				isSingleLineComment = false;
				if (!currString.empty())
				{
					tokens.push_back(parseString(currString));
					currString = "";
				}
				line++;
				charPos = 0;

				break;
			case '>':
				if (isSingleLineComment)
					break;

				if (!currString.empty()) {
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(GREATER_THAN));
				break;
			case '<':
				if (isSingleLineComment)
					break;

				if (!currString.empty()) {
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(LESS_THAN));
				break;
			case ';':
				if (isSingleLineComment)
					break;

				if (!currString.empty()) {
					tokens.push_back(parseString(currString));
					currString = "";
				}
				tokens.push_back(Token(SEMI_COLON));
				break;
			default:
				if (isSingleLineComment)
					break;

				if (std::isalnum(currChar) || currChar == '_') {
					if (std::isdigit(currChar) && inString == 'n' && currString.empty()) {
						tokens.push_back(buildNumber(&i));
					}
					else {
						currString += currChar;
					}
				} else {
					Logger::getInstance().throwException(UnexpectedSymbolException(line, charPos, currChar));
				}
				break;
			}

		}
		return tokens;
	}
}