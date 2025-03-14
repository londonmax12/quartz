#pragma once

#include "pch.hpp"

namespace Quartz {
    enum TokenType
    {
        NONE,

        STRING_VALUE,
        INT_VALUE,
        FLOAT_VALUE,

        KEYWORD_STRING,
        KEYWORD_INT,
        KEYWORD_FLOAT,
        KEYWORD_VOID,

        KEYWORD_STRATEGY,
        KEYWORD_CONST,
        KEYWORD_IF,
        KEYWORD_ELSE,
        KEYWORD_RETURN,
        
        KEYWORD_BUY,
        KEYWORD_SELL,
        KEYWORD_HOLD,

        KEYWORD_NULL,

        IDENTIFIER,
        OPEN_CURLY_BRACE,
        CLOSE_CURLY_BRACE,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        EQUALS,
        COLON,
        SEMI_COLON,
        COMMA,
        RIGHT_ARROW,
        GREATER_THAN,
        LESS_THAN,

        END,
        ERR,
    };

    struct Token {
        TokenType Type = NONE;
        std::any Data;

        int Line = 0;
        int CharPos = 0;

        Token() = default;

        Token(const TokenType& Type, const std::any& Data=NULL, const int Line = 0, const int CharPos = 0)
            : Type(Type), Data(Data), Line(Line), CharPos(CharPos)
        {
        }

        operator std::string() const {
            switch (Type) {
            case KEYWORD_STRATEGY:
                return "<STRATEGY_KEYWORD>";
            case KEYWORD_CONST:
                return "<CONST_KEYWORD>";
            case KEYWORD_IF:
                return "<IF_KEYWORD>";
            case KEYWORD_ELSE:
                return "<ELSE_KEYWORD>";
            case KEYWORD_RETURN:
                return "<RETURN_KEYWORD>";
            case KEYWORD_STRING:
                return "<STRING_KEYWORD>";
            case KEYWORD_INT:
                return "<INT_KEYWORD>";
            case KEYWORD_FLOAT:
                return "<FLOAT_KEYWORD>";
            case IDENTIFIER:
                return "<IDENTIFIER:" + std::any_cast<std::string>(Data) + ">";
            case STRING_VALUE:
                return "<STRING_VALUE:" + std::any_cast<std::string>(Data) + ">";
            case INT_VALUE:
                return "<INT_VALUE:" + std::any_cast<std::string>(Data) + ">";
            case FLOAT_VALUE:
                return "<FLOAT_VALUE:" + std::any_cast<std::string>(Data) + ">";
            case OPEN_CURLY_BRACE:
                return "<OPEN_CURLY_BRACE>";
            case CLOSE_CURLY_BRACE:
                return "<CLOSE_CURLY_BRACE>";
            case OPEN_BRACKET:
                return "<OPEN_BRACKET>";
            case CLOSE_BRACKET:
                return "<CLOSE_BRACKET>";
            case RIGHT_ARROW:
                return "<RIGHT_ARROW>";
            case EQUALS:
                return "<EQUALS>";
            case COLON:
                return "<COLON>";
            case SEMI_COLON:
                return "<SEMI_COLON>";
            case COMMA:
                return "<COMMA>";
            case KEYWORD_BUY:
                return "<BUY_KEYWORD>";
            case KEYWORD_HOLD:
                return "<HOLD_KEYWORD>";
            case KEYWORD_SELL:
                return "<SELL_KEYWORD>";
            case LESS_THAN:
                return "<LESS_THAN>";
            case GREATER_THAN:
                return "<GREATER_THAN>";
            case KEYWORD_NULL:
                return "<NULL_KEYWORD>";
            case KEYWORD_VOID:
                return "<VOID_KEYWORD>";
            default:
                return "<NONE:unknown_value>";
            }

        }
    };

    const std::unordered_map<std::string, TokenType> keywordMap = {
        {"strategy", KEYWORD_STRATEGY},
        {"const", KEYWORD_CONST},
        {"if", KEYWORD_IF},
        {"else", KEYWORD_ELSE},
        {"return", KEYWORD_RETURN},

        {"string", KEYWORD_STRING},
        {"float", KEYWORD_FLOAT},
        {"int", KEYWORD_INT},
        {"void", KEYWORD_VOID},

        {"null", KEYWORD_NULL},

        {"SELL", KEYWORD_SELL},
        {"BUY", KEYWORD_BUY},
        {"HOLD", KEYWORD_HOLD},
    };
}