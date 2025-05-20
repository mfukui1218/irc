#include "Logger.hpp"

/// @brief
Logger::Logger() {}

/// @brief
/// @param other
Logger::Logger(const Logger &other)
{
    // copy constructor
}

/// @brief
Logger::~Logger() {}

/// @brief
/// @param rhs
/// @return
Logger Logger::operator=(const Logger &rhs)
{
    if (this != &rhs)
    {
        // assignment logic
    }
    return *this;
}

void Logger::error(const std::string &) {}

void Logger::warn(const std::string &) {}

void Logger::debug(const std::string &) {}

void Logger::info(const std::string &) {}
