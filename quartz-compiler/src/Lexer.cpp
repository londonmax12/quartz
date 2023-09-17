#include "Lexer.h"

#include <iostream>

std::vector<Token> Lexer::Tokenize(const std::string &data) {
    std::vector<Token> tokens{};
    std::string currString{};

    for (int i = 0; i < data.length(); i++) {
        char c = data.at(i);
        if (std::isalpha(c)) {
            currString.push_back(c);
            i++;
            while (std::isalnum(data.at(i))) {
                currString.push_back(data.at(i));
                i++;
            }
            i--;

            if (currString == "return") {
                tokens.push_back(Token(TokenType::RETURN));
            }
            else {
                std::cerr << "Unexpected token\n";
                exit(1);
            }
            currString.clear();
        }
        else if (std::isdigit(c)) {
            currString.push_back(c);
            i++;
            while (std::isdigit(data.at(i))) {
                currString.push_back(data.at(i));
                i++;
            }
            i--;
            tokens.push_back(Token(TokenType::INT_LIT, currString));
            currString.clear();
        }
        else if (c == ';') {
            tokens.push_back(Token(TokenType::ENDL));
        }
        else if (std::isspace(c)) {
            continue;
        }
        else {
            std::cerr << "Unexpected symbol\n";
            exit(1);
        }
    }
    return tokens;
}
