#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

class Logger
{
    private:
        Logger();
        Logger(const Logger &other);
        virtual ~Logger();
        Logger operator=(const Logger &rhs);

    protected:
    public:
		void error(const std::string &msg);
		void warn(const std::string &msg);
		void debug(const std::string &msg);
		void info(const std::string &msg);
};

#endif // LOGGER_HPP
