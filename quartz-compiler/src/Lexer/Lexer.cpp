#include "Lexer.h"
#include "../Logging/Logging.h"

#include <sstream>
#include <fstream>

namespace Quartz {
    Lexer::Lexer(std::string src)
            : m_FilePath(src)
    {
    }

    Lexer::LexerOut Lexer::Tokenize() {
        std::ifstream file(m_FilePath, std::ios::binary | std::ios::ate);
        if (!file)
            throw LexerException(Logger::Format("Failed to open file {}", m_FilePath));

        std::ifstream::pos_type fileSize = file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0, std::ios::beg);
        if (!file.read(buffer.data(), fileSize))
            throw LexerException(Logger::Format("Failed to read file {}", m_FilePath));

        m_Src = std::string(buffer.begin(), buffer.end());

        std::string currString{};

        while (Peak()) {
            if (std::isalpha(Peak())) {
                int col = m_Column;
                currString.push_back(Consume());
                while (Peak() && isalnum(Peak())) {
                    currString.push_back(Consume());
                }
                if (currString == "exit") {
                    m_Out.Tokens.push_back({TokenType::EXIT, m_Line, col});
                } else if (currString == "var") {
                    m_Out.Tokens.push_back({TokenType::VAR, m_Line, col});
                } else if (currString == "int") {
                    m_Out.Tokens.push_back({TokenType::VAR_INT, m_Line, col});
                } else if (currString == "if") {
                    m_Out.Tokens.push_back({TokenType::IF, m_Line, col});
                } else {
                    m_Out.Tokens.push_back({TokenType::IDENTIFIER, m_Line, col, currString});
                }
                currString.clear();
                continue;
            }
            else if (std::isdigit(Peak())) {
                int col = m_Column;
                currString.clear();
                currString += Consume();
                while (Peak() && std::isdigit(Peak())) {
                    currString += Consume();
                }
                if (Peak() && std::isalnum(Peak())) {
                    throw LexerException("Indentifiers cannot begin with a number", {TokenType::ERROR, m_Line, m_Column});
                }
                m_Out.Tokens.push_back({TokenType::INT_LIT, m_Line, col, currString});
                currString.clear();
            }
            else if (Peak() == ';') {
                m_Out.Tokens.push_back({TokenType::ENDL, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '(') {
                m_Out.Tokens.push_back({TokenType::OPEN_PAREN, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == ')') {
                m_Out.Tokens.push_back({TokenType::CLOSE_PAREN, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '{') {
                m_Out.Tokens.push_back({TokenType::OPEN_CURLY, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '}') {
                m_Out.Tokens.push_back({TokenType::CLOSE_CURLY, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '=') {
                m_Out.Tokens.push_back({TokenType::EQUALS, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == ':') {
                m_Out.Tokens.push_back({TokenType::COLON, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '+') {
                m_Out.Tokens.push_back({TokenType::PLUS, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '*') {
                m_Out.Tokens.push_back({TokenType::MULTI, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '-') {
                m_Out.Tokens.push_back({TokenType::SUB, m_Line, m_Column});
                Consume();
            }
            else if (Peak() == '/') {
                m_Out.Tokens.push_back({TokenType::DIV, m_Line, m_Column});
                Consume();
            }
            else if (std::isspace(Peak())) {
                Consume();
            }
            else {
                std::stringstream eMsg;
                eMsg << "Unexpected symbol: " << Peak();
                throw LexerException(eMsg.str(), {TokenType::ERROR, m_Line, m_Column});
            }
        }

        m_Index = 0;
        return m_Out;
    }

    char Lexer::Peak(int offset) {
        if (m_Index + offset >= m_Src.length()) {
            return (char)0;
        }
        return m_Src[m_Index + offset];
    }

    char Lexer::Consume() {
        char currentChar = m_Src[m_Index++];
        if (currentChar == '\n') {
            m_Line++;
            m_Column = 1;
        } else {
            m_Column++;
        }

        return currentChar;
    }
}