#include "Lexer.h"

#include <iostream>

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
            else {
                std::cerr << "Unexpected identifier: " << currString << "\n";
                exit(1);
            }
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

Lexer::Lexer(std::string src)
    : m_Src(src)
{
}

char Lexer::Peak(int ahead) {
    if (m_Index + ahead > m_Src.length()) {
        return (char)0;
    }
    return m_Src[m_Index];
}

char Lexer::Consume() {
    return m_Src[m_Index++];
}
