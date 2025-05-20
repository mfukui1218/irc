#include "CommandFactory.hpp"

/// @brief 
CommandFactory::CommandFactory()
{
}

/// @brief 
/// @param other 
CommandFactory::CommandFactory(const CommandFactory &other)
{
	// copy constructor
}

/// @brief 
CommandFactory::~CommandFactory()
{
}

/// @brief 
/// @param rhs 
/// @return 
CommandFactory CommandFactory::operator=(const CommandFactory &rhs)
{
	if (this != &rhs) {
		// assignment logic
	}
	return *this;
}

ICommand *CommandFactory::createCommand(const std::string &)
{
	return (NULL);
}
