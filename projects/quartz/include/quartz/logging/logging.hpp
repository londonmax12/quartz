#pragma once

#include "../../pch.hpp"

class PositionalException : public std::exception {
protected:
    int line;
    int charpos;
    std::string message;

public:
    PositionalException(int line, int charpos, const std::string& msg)
        : line(line), charpos(charpos) 
    {
        std::ostringstream oss;
        oss << "Error at line " << line << ", char " << charpos << ": " << msg;
        message = oss.str();
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

    int getLine() const { return line; }
    int getCharPos() const { return charpos; }
};

class UnexpectedSymbolException : public PositionalException {
public:
    UnexpectedSymbolException(int line, int charpos, char symbol)
        : PositionalException(line, charpos, "Unexpected symbol '" + std::string(1, symbol) + "'") {}
};

namespace Quartz {
    class Logger {
    public:
        enum Level { DEBUG, INFO, WARNING, ERROR, FATAL};

        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        void log(Level level, const std::string& message);

        void setLevel(Level level) { currentLevel = level; }

        void setOutputStream(std::ostream& outputStream) { out = &outputStream; }

        template <typename... Args>
        void logf(Level level, const std::string& format, Args... args) {
            if (level >= currentLevel) {
                log(level, formatMessage(format, args...));
            }
        }

        void throwException(const std::exception& exeption);

    private:
        std::ostream* out;
        std::mutex mutex;
        Level currentLevel = DEBUG;

        Logger() : out(&std::cout) {}

        std::string levelToString(Level level) {
            switch (level) {
            case DEBUG:   return "DEBUG";
            case INFO:    return "INFO";
            case WARNING: return "WARNING";
            case ERROR:   return "ERROR";
            case FATAL:   return "FATAL";
            default:      return "UNKNOWN";
            }
        }

        std::string getCurrentTime();

        template <typename... Args>
        std::string formatMessage(const std::string& format, Args... args) {
            size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
            std::unique_ptr<char[]> buf(new char[size]);
            snprintf(buf.get(), size, format.c_str(), args...);
            return std::string(buf.get(), buf.get() + size - 1);
        }
    };
}