#ifndef QUARTZ_LOGGING_H
#define QUARTZ_LOGGING_H

#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>

#include "../Lexer/Token.h"

#define LOG_TRACE(fmt, ...) Quartz::Logger::Log(Quartz::Logger::LOG_TRACE, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Quartz::Logger::Log(Quartz::Logger::LOG_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) Quartz::Logger::Log(Quartz::Logger::LOG_WARNING, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Quartz::Logger::Log(Quartz::Logger::LOG_ERROR, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) Quartz::Logger::Log(Quartz::Logger::LOG_FATAL, fmt, ##__VA_ARGS__)

namespace Quartz {
    class Logger {
    public:
        enum LoggerCode {
            LOG_TRACE,
            LOG_INFO,
            LOG_WARNING,
            LOG_ERROR,
            LOG_FATAL
        };

        template<typename... Args>
        static void Log(LoggerCode code, std::string fmt, Args... args) {
            std::string time = GetTimeAsString();
            std::string codeString;
            std::string msg = Format(fmt, std::forward<Args>(args)...);

            switch (code) {
                case LOG_TRACE:
                    codeString = "[TRACE]:   ";
                    break;
                case LOG_INFO:
                    codeString = "[INFO]:    ";
                    break;
                case LOG_WARNING:
                    codeString = "[WARNING]: ";
                    break;
                case LOG_ERROR:
                    codeString = "[ERROR]:   ";
                    break;
                case LOG_FATAL:
                    codeString = "[FATAL]:   ";
                    break;
            }

            if (code > LOG_WARNING) {
                std::cerr << time << codeString << msg << "\n";
            } else {
                std::cout << time << codeString << msg << "\n";
            }
        }

        template<typename... Args>
        static std::string Format(const std::string& fmt, Args... args) {
            return FormatHelper(fmt, args...);
        }
    private:
        static std::string GetTimeAsString() {
            time_t time = std::time(nullptr);
            auto localTime = *std::localtime(&time);

            std::ostringstream oss;
            oss << std::put_time(&localTime, "[%H:%M:%S]");
            return oss.str();
        }

        template<typename T>
        static std::string ConvertToString(const T& value) {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }

        static std::string FormatHelper(const std::string& fmt) {
            return fmt;
        }

        template<typename T, typename... Args>
        static std::string FormatHelper(const std::string& fmt, T value, Args... args) {
            size_t start_pos = fmt.find("{");
            size_t end_pos = fmt.find("}", start_pos);
            if (start_pos != std::string::npos && end_pos != std::string::npos) {
                std::string result = fmt.substr(0, start_pos);
                result += ConvertToString(value);
                result += fmt.substr(end_pos + 1);
                return FormatHelper(result, args...);
            }
            return fmt;
        }
    };
}
#endif //QUARTZ_LOGGING_H
