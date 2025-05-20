#ifndef COMMAND_FACTORY_HPP
# define COMMAND_FACTORY_HPP

#include "ICommand.hpp"
#include <string>

class CommandFactory
{
	private:
	protected:
	public:
		CommandFactory();
		CommandFactory(const CommandFactory &other);
		virtual ~CommandFactory();
		CommandFactory operator=(const CommandFactory &rhs);

		ICommand *createCommand(const std::string &line);
};

#endif // COMMAND_FACTORY_HPP
