#include "Token.h"

namespace Quartz {
    Token::Token(TokenType type, std::string value)
            : m_Type(type), m_Value(value)
    {
    }

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
        if (m_Type == TokenType::NONE)
            return false;

        return true;
    }

    bool Token::HasBinPrec() {
        if (GetBinPrec(m_Type) == -1)
            return false;

        return true;
    }

    std::string Token::Str() {
        switch (m_Type) {
            case EXIT:
                return "EXIT";
            case INT_LIT:
                return "INT_LIT: " + m_Value;
            case ENDL:
                return "ENDL";
            case OPEN_PAREN:
                return "OPEN_PAREN";
            case CLOSE_PAREN:
                return "CLOSE_PAREN";
            case IDENTIFIER:
                return "IDENTIFIER: " + m_Value;
            case VAR:
                return "VAR";
            case VAR_INT:
                return "VAR_INT";
            case EQUALS:
                return "EQUALS";
            case PLUS:
                return "PLUS";
            case MULTI:
                return "MULTI";
            case COLON:
                return "COLON";
            case SUB:
                return "SUB";
            case DIV:
                return "DIV";
            case NONE:
                return "NONE";
            default:
                return "UNKNOWN";
        }
    }
}