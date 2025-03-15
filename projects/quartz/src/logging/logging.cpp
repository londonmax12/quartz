#include  "logging/logging.hpp"

void Quartz::Logger::log(Level level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (level >= printLevel) {
        *out << getCurrentTime() << " [" << levelToString(level) << "] " << message << std::endl;
    }
}

void Quartz::Logger::throwException(const std::exception& exception)
{
    logf(FATAL, "Exception: %s", exception.what());
    throw exception;
}

std::string Quartz::Logger::getCurrentTime()
{
    std::time_t now = std::time(nullptr);
    std::tm* tm_info = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm_info, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
