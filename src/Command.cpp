#include "Command.hpp"

Command::Command() {}

Command::~Command() {}

void Command::execute(Client& client, Server& server, const std::string& message)
{
	std::string cmd, args;
	std::pair<std::string, std::string> result = parseCommand(message);
	cmd = result.first;
	args = result.second;

    if (cmd == "PASS")
        handlePass(client, server, args);
    else if (cmd == "NICK")
        handleNick(client, server, args);
    else if (cmd == "USER")
        handleUser(client, args);
    else if (cmd == "JOIN")
        handleJoin(client, server, args);
    else if (cmd == "PRIVMSG")
        handlePrivmsg(client, server, args);
    else
        sendError(client, ERR_UNKNOWNCOMMAND);
}

void handlePass(Client& client, Server& server, const std::string& args)
{
	if (client.isAuthenticated()) {
        sendError(client, ERR_ALREADYREGISTERED);
        return;
    }
	if (args != server.getPassword()) {
        sendError(client, ERR_PASSWDMISMATCH);
        return;
    }
	client.setAuthenticated(true);
	std::cout << "Handling PASS command with args: " << args << std::endl;
}

void handleNick(Client& client, Server& server, const std::string& args)
{
	if (args.empty()) {
		sendError(client, ERR_NONICKNAMEGIVEN);
		return;
	}
	if (args.length() > 9) {
		sendError(client, ERR_ERRONEUSNICKNAME);
		return;
	}
	if (isalpha(args[0]) == 0 && std::string("[\\`_^{}|]").find(args[0]) == std::string::npos)
	{
		sendError(client, ERR_ERRONEUSNICKNAME);
		return;
	}
	for (size_t i = 1; i < args.size(); ++i) {
        if (!isalnum(args[i]) && std::string("-[\\`_^{}|]").find(args[i]) == std::string::npos)
		{
			sendError(client, ERR_ERRONEUSNICKNAME);
			return;
		}
    }
	if (server.isNicknameInUse(args))
	{
		sendError(client, ERR_NICKNAMEINUSE);
    	return;
	}
	client.setNickname(args);
	client.setNicknameRegistered(true);
	std::cout << "Handling NICK command with args: " << args << std::endl;
}

void handleUser(Client& client, const std::string& args)
{
	std::istringstream iss(args);
    std::string username, hostname, servername, realname;
    iss >> username >> hostname >> servername;
    std::getline(iss, realname);
    if (username.empty() || hostname.empty() || servername.empty()) {
        sendError(client, ERR_NEEDMOREPARAMS);
        return;
	}
	realname.erase(0, realname.find_first_not_of(' '));
	if (realname.empty() || realname[0] != ':')
	{
		sendError(client, ERR_NEEDMOREPARAMS);
		return;
	}
	realname = realname.substr(1);
	if (client.isUserRegistered()) {
		sendError(client, ERR_ALREADYREGISTERED);
		return;
	}
	client.setUsername(username);
	client.setRealname(realname);
	client.setUserRegistered(true);
	std::cout << "Handling USER command with args: " << args << std::endl;
}

void handleJoin(Client& client, Server& server, const std::string& args)
{
	if (args.empty()) {
		sendError(client, ERR_NEEDMOREPARAMS);
		return;
	}
	std::istringstream iss(args);
    std::string channelName;
    while (std::getline(iss, channelName, ',')) {
        if (channelName[0] != '#' && channelName[0] != '&') {
            sendError(client, ERR_NOSUCHCHANNEL);
            continue;
        }
        Channel* channel = server.findChannel(channelName);
        if (!channel) {
            channel = new Channel(channelName);
            server.addChannel(channel);
            channel->addOperator(&client);
        }
        if (!channel->hasClient(&client)) {
            channel->addClient(&client);
            client.addJoinedChannel(channelName);
            channel->broadcast(":" + client.getNickname() + " JOIN " + channelName);
        }
    }
	std::cout << "Handling JOIN command with args: " << args << std::endl;
}

void handlePrivmsg(Client& client, Server& server, const std::string& args)
{
	if (args.empty()) {
		sendError(client, ERR_NEEDMOREPARAMS);
		return;
	}
	std::istringstream iss(args);
	std::string target, message;
	iss >> target;
	std::getline(iss, message);
	if (!message.empty() && message[0] == ' ')
        message.erase(0, 1);
    if (!message.empty() && message[0] == ':')
        message.erase(0, 1);
    if (target.empty()) {
        sendError(client, ERR_NORECIPIENT);
        return;
    }
	if (message.empty()) {
        sendError(client, ERR_NOTEXTTOSEND);
        return;
    }
	if (target[0] == '#' || target[0] == '&') {
        Channel* channel = server.findChannel(target);
        if (!channel) {
            sendError(client, ERR_NOSUCHNICK);
            return;
        }
		if (!channel->hasClient(&client)) {
            sendError(client, ERR_CANNOTSENDTOCHAN);
            return;
        }
        std::set<Client*> clients = channel->getClients();
        for (std::set<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
            if (*it != &client) {
                (*it)->sendMessage(":" + client.getNickname() + " PRIVMSG " + target + " :" + message);
            }
        }
    } else {
        Client* targetClient = server.findClientByNickname(target);
        if (!targetClient) {
            sendError(client, ERR_NOSUCHNICK);
            return;
        }
        targetClient->sendMessage(":" + client.getNickname() + " PRIVMSG " + target + " :" + message);
    }
	std::cout << "Handling PRIVMSG command with args: " << args << std::endl;
}

std::pair<std::string, std::string> parseCommand(const std::string& message)
{
	std::istringstream iss(message);
	std::string cmd, args;
	iss >> cmd;
	std::getline(iss, args);
	if (!args.empty() && args[0] == ' ')
		args.erase(0, 1);
	return std::make_pair(cmd, args);
}

void sendError(Client& client, const int errorCode)
{
	std::string errorMessage;
	switch (errorCode) {
		case ERR_ALREADYREGISTERED:
			errorMessage = "ERROR: Already registered\n";
			break;
		case ERR_NEEDMOREPARAMS:
			errorMessage = "ERROR: Need more parameters\n";
			break;
		case ERR_PASSWDMISMATCH:
			errorMessage = "ERROR: Password mismatch\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			errorMessage = "ERROR: No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			errorMessage = "ERROR: Erroneous nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			errorMessage = "ERROR: Nickname already in use\n";
			break;
		case ERR_NOTREGISTERED:
			errorMessage = "ERROR: Not registered\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			errorMessage = "ERROR: Unknown command\n";
			break;
		case ERR_NOTEXTTOSEND:
			errorMessage = "ERROR: No text to send\n";
			break;
		case ERR_NORECIPIENT:
			errorMessage = "ERROR: No recipient\n";
			break;
		case ERR_NOTOPLEVEL:
			errorMessage = "ERROR: No top level domain\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			errorMessage = "ERROR: Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			errorMessage = "ERROR: Too many channels\n";
			break;
		case ERR_UNKNOWNMODE:
			errorMessage = "ERROR: Unknown mode\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			errorMessage = "ERROR: Channel operator privileges needed\n";
			break;
		case ERR_USERONCHANNEL:
			errorMessage = "ERROR: User already on channel\n";
			break;
		case ERR_NOSUCHNICK:
			errorMessage = "ERROR: No such nickname\n";
			break;
		case ERR_NOSUCHCHANNEL:
			errorMessage = "ERROR: No such channel\n";
			break;
		default:
			errorMessage = "ERROR: Unknown error code\n";
			break;
	}
	client.sendMessage(errorMessage);
}

