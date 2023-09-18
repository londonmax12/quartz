#include "Token.h"

Token::Token(TokenType type, std::string value)
    : m_Type(type), m_Value(value)
{
}

std::string Token::Str() {
    switch (m_Type) {
        case TokenType::ENDL: {
            return "ENDL";
            break;
        }
        case TokenType::INT_LIT: {
            return "INT_LITERAL: " + m_Value;
            break;
        }
        case TokenType::EXIT: {
                return "EXIT";
                break;
        }
        default: {
            return "UNDEFINED";
        }
    }
}
