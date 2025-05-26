#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include <iostream>
#include <sstream>

class Server; // Forward declaration
class Client; // Forward declaration

class Command {
public:
	Command();
	virtual ~Command();
	virtual void execute(Client& client, Server& server, const std::string& message);
};

enum ErrorCode {
    ERR_ALREADYREGISTERED = 462,
    ERR_NEEDMOREPARAMS = 461,
    ERR_PASSWDMISMATCH = 464,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NOTREGISTERED = 451,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NOTEXTTOSEND = 412,
	ERR_NORECIPIENT = 411,
	ERR_NOTOPLEVEL = 413,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_UNKNOWNMODE = 472,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_USERONCHANNEL = 443,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
};

void handlePass(Client& client, Server& server, const std::string& args);
void handleNick(Client& client, Server& server, const std::string& args);
void handleUser(Client& client, const std::string& args);
void handleJoin(Client& client, Server& server, const std::string& args);
void handlePrivmsg(Client& client, Server& server, const std::string& args);
void sendError(Client& client, const int errorCode, const std::string& command = "", const std::string& target = "");
void handleMode(Client& client, Server& server, const std::string& args);
void handleKick(Client& client, Server& server, const std::string& args);
void handleTopic(Client& client, Server& server, const std::string& args);
void handleInvite(Client& client, Server& server, const std::string& args);
void handlePart(Client& client, Server& server, const std::string& args);
void handlePing(Client& client, const std::string& args);
std::pair<std::string, std::string> parseCommand(const std::string& message);

#endif
