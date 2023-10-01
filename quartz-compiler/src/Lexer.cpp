#include "Lexer.h"

#include <iostream>

Lexer::Lexer(std::string src)
        : m_Src(src)
{
}

std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens{};
    std::string currString{};

    while (Peak()) {
        if (std::isalpha(Peak())) {
            currString.push_back(Consume());
            while (Peak() && isalnum(Peak())) {
                currString.push_back(Consume());
            }
            if (currString == "exit") {
                tokens.push_back({TokenType::EXIT});
                currString.clear();
                continue;
            }
            else if (currString == "var") {
                tokens.push_back({TokenType::VAR});
                currString.clear();
                continue;
            }
            else if (currString == "int") {
                tokens.push_back({TokenType::VAR_INT});
                currString.clear();
                continue;
            }

            tokens.push_back({TokenType::IDENTIFIER, currString});
            currString.clear();
            continue;
        }
        else if (std::isdigit(Peak())) {
            currString += Consume();
            while (Peak() && std::isdigit(Peak())) {
                currString += Consume();
            }
            tokens.push_back({TokenType::INT_LIT, currString});
            currString.clear();
            continue;
        }
        else if (Peak() == ';') {
            Consume();
            tokens.push_back({TokenType::ENDL});
            continue;
        }
        else if (Peak() == '(') {
            Consume();
            tokens.push_back({TokenType::OPEN_PAREN});
            continue;
        }
        else if (Peak() == ')') {
            Consume();
            tokens.push_back({TokenType::CLOSE_PAREN});
            continue;
        }
        else if (Peak() == '=') {
            Consume();
            tokens.push_back({TokenType::EQUALS});
            continue;
        }
        else if (Peak() == ':') {
            Consume();
            tokens.push_back({TokenType::COLON});
            continue;
        }
        else if (Peak() == '+') {
            Consume();
            tokens.push_back({TokenType::PLUS});
            continue;
        }
        else if (std::isspace(Peak())) {
            Consume();
            continue;
        }
        else {
            std::cerr << "Unexpected symbol\n";
            exit(1);
        }
    }

    m_Index = 0;
    return tokens;
}

char Lexer::Peak(int offset) {
    if (m_Index + offset >= m_Src.length()) {
        return (char)0;
    }
    return m_Src[m_Index + offset];
}

char Lexer::Consume() {
    return m_Src[m_Index++];
}