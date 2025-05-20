#include "CommandSample.hpp"

/// @brief 
CommandSample::CommandSample()
{
}

/// @brief 
/// @param other 
CommandSample::CommandSample(const CommandSample &other)
{
	// copy constructor
}

/// @brief 
CommandSample::~CommandSample()
{
}

/// @brief 
/// @param rhs 
/// @return 
CommandSample CommandSample::operator=(const CommandSample &rhs)
{
	if (this != &rhs) {
		// assignment logic
	}
	return *this;
}

CommandSample::CommandSample(std::string args)
	: channel_etc(args)
{

}

void CommandSample::execute(Session *session, IRCServer *server)
{

}
