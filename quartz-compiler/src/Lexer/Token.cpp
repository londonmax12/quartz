#include "Token.h"

namespace Quartz {
    Token::Token(TokenType type, std::string value)
            : m_Type(type), m_Value(value) {}

    Token::Token(TokenType type, int line, int col, std::string value)
            : m_Type(type), m_Value(value), m_Line(line), m_Column(col) {}

    bool Token::IsBinOperator() {
        return IsBinOperator(m_Type);
    }

    bool Token::IsBinOperator(TokenType type) {
        switch (type) {
            case TokenType::PLUS:
            case TokenType::MULTI:
            case TokenType::DIV:
            case TokenType::SUB:
                return true;
            default:
                return false;
        }
        return false;
    }

    int Token::GetBinPrec() {
        return GetBinPrec(m_Type);
    }

    int Token::GetBinPrec(TokenType type) {
        switch (type) {
            case TokenType::PLUS:
            case TokenType::SUB:
                return 0;
            case TokenType::MULTI:
            case TokenType::DIV:
                return 1;
            default:
                return -1;
        }
    }

    bool Token::IsValid() {
        if (m_Type >= TokenType::ERROR)
            return false;

        return true;
    }

    bool Token::HasBinPrec() {
        if (GetBinPrec(m_Type) == -1)
            return false;

        return true;
    }

    std::ostream& operator<<(std::ostream& os, const Quartz::Token& token) {
        os << "Line: " << token.GetLine() << ", Col: " << token.GetColumn();
        return os;
    }
}