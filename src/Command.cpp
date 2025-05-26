#include "Command.hpp"

Command::Command() {}

Command::~Command() {}

void Command::execute(Client& client, Server& server, const std::string& message)
{
	std::string cmd, args;
	std::pair<std::string, std::string> result = parseCommand(message);
	cmd = result.first;
	args = result.second;

	if (cmd.empty()) {
		sendError(client, ERR_NEEDMOREPARAMS, "", "");
		return;
	}
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
	else if (cmd == "MODE")
		handleMode(client, server, args);
	else if (cmd == "KICK")
		handleKick(client, server, args);
	else if (cmd == "TOPIC")
		handleTopic(client, server, args);
	else if (cmd == "INVITE")
		handleInvite(client, server, args);
	else if (cmd == "PART")
		handlePart(client, server, args);
	else if (cmd == "PING")
		handlePing(client, args);
    else
        sendError(client, ERR_UNKNOWNCOMMAND);
}

void handlePass(Client& client, Server& server, const std::string& args)
{
	if (client.isNicknameRegistered() || client.isUserRegistered()) {
        sendError(client, ERR_ALREADYREGISTERED);
        return;
	}
	if (args.empty()) {
		sendError(client, ERR_NEEDMOREPARAMS, "PASS");
		return;
	}
	if (args != server.getPassword()) {
        sendError(client, ERR_PASSWDMISMATCH);
        return;
    }
	client.setPassAccepted(true);
	std::cout << "Handling PASS command with args: " << args << std::endl;
}

void handleNick(Client& client, Server& server, const std::string& args)
{
	if (args.empty()) {
		sendError(client, ERR_NONICKNAMEGIVEN, "NICK");
		return;
	}
	if (args.length() > 9) {
		sendError(client, ERR_ERRONEUSNICKNAME, "NICK", args);
		return;
	}
	if (!client.isPassAccepted()) {
		sendError(client, ERR_NOTREGISTERED, "NICK");
		return;
	}
	if (isalpha(args[0]) == 0 && std::string("[\\`_^{}|]").find(args[0]) == std::string::npos) {
		sendError(client, ERR_ERRONEUSNICKNAME, "NICK", args);
		return;
	}
	for (size_t i = 1; i < args.size(); ++i) {
		if (!isalnum(args[i]) && std::string("-[\\`_^{}|]").find(args[i]) == std::string::npos) {
			sendError(client, ERR_ERRONEUSNICKNAME, "NICK", args);
			return;
		}
	}
	if (server.isNicknameInUse(args)) {
		sendError(client, ERR_NICKNAMEINUSE, "NICK", args);
		return;
	}

	client.setNickname(args);
	client.setNicknameRegistered(true);

	if (client.isAuthenticated()) {
		client.setAuthenticated(true);
		client.sendMessage(":irc.localhost 001 " + client.getNickname() + " :Welcome to the server!\r\n");
	}

	std::cout << "Handling NICK command with args: " << args << std::endl;
}

void handleUser(Client& client, const std::string& args)
{
	std::istringstream iss(args);
	std::string username, hostname, servername, realname;
	iss >> username >> hostname >> servername;
	std::getline(iss, realname);

	if (username.empty() || hostname.empty() || servername.empty()) {
		sendError(client, ERR_NEEDMOREPARAMS, "USER");
		return;
	}

	realname.erase(0, realname.find_first_not_of(' '));
	if (realname.empty() || realname[0] != ':') {
		sendError(client, ERR_NEEDMOREPARAMS, "USER");
		return;
	}

	realname = realname.substr(1);

	if (client.isUserRegistered()) {
		sendError(client, ERR_ALREADYREGISTERED, "USER");
		return;
	}

	client.setUsername(username);
	client.setRealname(realname);
	client.setUserRegistered(true);

	if (client.isAuthenticated()) {
		client.setAuthenticated(true);
		client.sendMessage(":irc.localhost 001 " + client.getNickname() + " :Welcome to the server!\r\n");
	}

	std::cout << "Handling USER command with args: " << args << std::endl;
}

void handleJoin(Client& client, Server& server, const std::string& args)
{
	if (!client.isAuthenticated()) {
		sendError(client, ERR_NOTREGISTERED, "JOIN");
		return;
	}
	if (args.empty()) {
		sendError(client, ERR_NEEDMOREPARAMS, "JOIN");
		return;
	}
	if (client.getJoinedChannels().size() >= 10) {
		sendError(client, ERR_TOOMANYCHANNELS, "JOIN");
		return;
	}

	std::istringstream iss(args);
	std::string channelName;
	while (std::getline(iss, channelName, ',')) {
		if (channelName[0] != '#' && channelName[0] != '&') {
			sendError(client, ERR_NOSUCHCHANNEL, "JOIN", channelName);
			continue;
		}

		Channel* channel = server.findChannel(channelName);
		if (!channel) {
			channel = new Channel(channelName);
			server.addChannel(channel);
			channel->addOperator(client);
			client.addOperatorChannel(channelName);
		}
		else {
			if (channel->isInviteOnly() && !channel->isInvited(&client)) {
				sendError(client, 473, "JOIN", channelName); // 473 = ERR_INVITEONLYCHAN
				continue;
			}
			if (channel->isLimitEnabled() && channel->getUserCount() >= channel->getUserLimit()) {
				sendError(client, 471, "JOIN", channelName); // 471 = ERR_CHANNELISFULL
				continue;
			}
			if (channel->hasClient(&client)) {
				continue; // すでに入ってる
			}
		}

		// 正常に参加処理
		channel->addClient(&client);
		client.addJoinedChannel(channelName);

		// RFC準拠のJOIN通知（:nick!user@host JOIN :#channel）
		channel->broadcast(":" + client.getFullIdentifier() + " JOIN :" + channelName + "\r\n");

		// 参加後、TOPICやNAMESなども返すならここで
		server.joinChannel(client, channelName);
	}
	std::cout << "Handling JOIN command with args: " << args << std::endl;
}

void handlePrivmsg(Client& client, Server& server, const std::string& args)
{
	if (!client.isAuthenticated()) {
		sendError(client, ERR_NOTREGISTERED, "PRIVMSG");
		return;
	}
	if (args.empty()) {
		sendError(client, ERR_NEEDMOREPARAMS, "PRIVMSG");
		return;
	}

	std::istringstream iss(args);
	std::string target, message;
	iss >> target;
	std::getline(iss, message);

	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);

	if (target.empty()) {
		sendError(client, ERR_NORECIPIENT, "PRIVMSG");
		return;
	}
	if (message.empty()) {
		sendError(client, ERR_NOTEXTTOSEND, "PRIVMSG");
		return;
	}

	// チャンネル宛てのメッセージ
	if (target[0] == '#' || target[0] == '&') {
		Channel* channel = server.findChannel(target);
		if (!channel) {
			sendError(client, ERR_NOSUCHNICK, "PRIVMSG", target); // RFC的には ERR_NOSUCHCHANNEL でも可
			return;
		}
		if (!channel->hasClient(&client)) {
			sendError(client, ERR_CANNOTSENDTOCHAN, "PRIVMSG", target);
			return;
		}

		std::set<Client*> clients = channel->getClients();
		for (std::set<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
			if (*it != &client) {
				(*it)->sendMessage(":" + client.getFullIdentifier() + " PRIVMSG " + target + " :" + message + "\r\n");
			}
		}
	}
	else {
		Client* targetClient = server.findClientByNickname(target);
		if (!targetClient) {
			sendError(client, ERR_NOSUCHNICK, "PRIVMSG", target);
			return;
		}
		targetClient->sendMessage(":" + client.getFullIdentifier() + " PRIVMSG " + target + " :" + message + "\r\n");
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

void handleMode(Client& client, Server& server, const std::string& args) {
    std::stringstream ss(args);
    std::string channelName, mode;
    ss >> channelName >> mode;

    Channel* channel = server.findChannel(channelName);
	if (!client.isAuthenticated()) {
		client.sendMessage("451 JOIN :You have not registered\n");
		return;
	}
    if (!channel) {
        client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
        return;
    }
	if (mode.empty()) {
		client.sendMessage("461 " + client.getNickname() + " " + channelName + " :Not enough parameters\n");
		return;
	}
	if (mode[0] != '+' && mode[0] != '-') {
		client.sendMessage("472 " + client.getNickname() + " " + channelName + " :Unknown mode\n");
		return;
	}
	if (mode.length() <= 1) {
		client.sendMessage("461 " + client.getNickname() + " " + channelName + " :Not enough parameters\n");
		return;
	}
	std::string remainingArgs;
    std::getline(ss, remainingArgs);
    std::stringstream argStream(remainingArgs);
    if (!client.isOperatorOf(channelName)) {
        client.sendMessage("482 " + channelName + " :You're not channel operator\n");
        return;
    }
    channel->applyMode(mode, argStream, client, server);
	channel->broadcast(":" + client.getFullIdentifier() + " MODE " + channelName + " " + mode + " " + remainingArgs + "\n");
}

void handleKick(Client& client, Server& server, const std::string& args) {
	std::stringstream ss(args);
	std::string channelName, targetNickname;
	ss >> channelName >> targetNickname;

	if (!client.isAuthenticated()) {
		client.sendMessage("451 JOIN :You have not registered\n");
		return;
	}
	if (channelName.empty() || targetNickname.empty()) {
		client.sendMessage("461 " + client.getNickname() + " " + channelName + " :Not enough parameters\n");
		return;
	}
	if (channelName[0] != '#' && channelName[0] != '&') {
		client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}
	Channel* channel = server.findChannel(channelName);
	if (!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}
	if (!client.isOperatorOf(channelName)) {
		client.sendMessage("482 " + channelName + " :You're not channel operator\n");
		return;
	}
	Client* targetClient = server.findClientByNickname(targetNickname);
	if (!targetClient) {
		client.sendMessage("441 " + client.getNickname() + " " + targetNickname + " :They aren't on that channel\n");
		return;
	}
	channel->removeClient(targetClient);
	targetClient->removeJoinedChannel(channelName);
	if (targetClient->isOperatorOf(channelName)) {
		targetClient->removeOperatorChannel(channelName);
	}
	targetClient->sendMessage(":" + client.getFullIdentifier() + " KICK " + channelName + " " + targetNickname + "\n");
	channel->broadcast(":" + client.getFullIdentifier() + " KICK " + channelName + " " + targetNickname + "\n");
}

void handleTopic(Client& client, Server& server, const std::string& args) {
	std::stringstream ss(args);
	std::string channelName, topic;
	ss >> channelName;
	std::getline(ss, topic);
	if (!client.isAuthenticated()) {
		client.sendMessage("451 JOIN :You have not registered\n");
		return;
	}
	if (!topic.empty() && topic[0] == ' ')
		topic.erase(0, 1);
	Channel* channel = server.findChannel(channelName);
	if (!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}
	if (!client.isOperatorOf(channelName)) {
		client.sendMessage("482 " + channelName + " :You're not channel operator\n");
		return;
	}
	channel->setTopic(topic);
	channel->broadcast(":" + client.getFullIdentifier() + " TOPIC " + channelName + " :" + topic + "\n");
}

void handleInvite(Client& client, Server& server, const std::string& args) {
	std::stringstream ss(args);
	std::string targetNickname, channelName;
	ss >> targetNickname >> channelName;

	if (!client.isAuthenticated()) {
		client.sendMessage("451 JOIN :You have not registered\n");
		return;
	}
	if (targetNickname.empty() || channelName.empty()) {
		client.sendMessage("461 " + client.getNickname() + " " + channelName + " :Not enough parameters\n");
		return;
	}
	if (channelName[0] != '#' && channelName[0] != '&') {
		client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}
	Channel* channel = server.findChannel(channelName);
	if (!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}
	Client* targetClient = server.findClientByNickname(targetNickname);
	if (!targetClient) {
		client.sendMessage("441 " + client.getNickname() + " " + targetNickname + " :They aren't on that channel\n");
		return;
	}
	channel->invite(targetClient);
	targetClient->sendMessage(":" + client.getFullIdentifier() + " INVITE " + targetNickname + " :" + channelName + "\n");
	std::cout << "Handling INVITE command with args: " << args << std::endl;
}

void handlePart(Client& client, Server& server, const std::string& args) {
	std::stringstream ss(args);
	std::string channelName;
	ss >> channelName;

	if (!client.isAuthenticated()) {
		client.sendMessage("451 PART :You have not registered\n");
		return;
	}
	if (channelName.empty()) {
		client.sendMessage("461 " + client.getNickname() + " :Not enough parameters\n");
		return;
	}
	if (channelName[0] != '#' && channelName[0] != '&') {
		client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}
	Channel* channel = server.findChannel(channelName);
	if (!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}
	if (!channel->hasClient(&client)) {
		client.sendMessage("442 " + client.getNickname() + " " + channelName + " :You're not on that channel\n");
		return;
	}
	channel->removeClient(&client);
	client.removeJoinedChannel(channelName);
	if (client.isOperatorOf(channelName)) {
		client.removeOperatorChannel(channelName);
	}
	channel->broadcast(":" + client.getFullIdentifier() + " PART " + channelName + "\n");
	std::cout << "Handling PART command with args: " << args << std::endl;
}

void handlePing(Client& client, const std::string& args) {
	if (args.empty()) {
		client.sendMessage("409 " + client.getNickname() + " :No server specified for PING\n");
		return;
	}
	client.sendMessage("PONG :" + args + "\r\n");
	std::cout << "Handling PING command with args: " << args << std::endl;
}

void sendError(Client& client, int errorCode, const std::string& command, const std::string& target) {
	std::string reply;
	std::string nick = client.getNickname();
	std::string serverName = "irc.localhost";

	switch (errorCode) {
		case ERR_ALREADYREGISTERED:
			reply = ":" + serverName + " 462 " + nick + " :You may not reregister";
			break;
		case ERR_NEEDMOREPARAMS:
			reply = ":" + serverName + " 461 " + nick + " " + command + " :Not enough parameters";
			break;
		case ERR_PASSWDMISMATCH:
			reply = ":" + serverName + " 464 " + nick + " :Password incorrect";
			break;
		case ERR_NONICKNAMEGIVEN:
			reply = ":" + serverName + " 431 " + nick + " :No nickname given";
			break;
		case ERR_ERRONEUSNICKNAME:
			reply = ":" + serverName + " 432 " + nick + " " + target + " :Erroneous nickname";
			break;
		case ERR_NICKNAMEINUSE:
			reply = ":" + serverName + " 433 " + nick + " " + target + " :Nickname is already in use";
			break;
		case ERR_NOTREGISTERED:
			reply = ":" + serverName + " 451 " + nick + " :You have not registered";
			break;
		case ERR_UNKNOWNCOMMAND:
			reply = ":" + serverName + " 421 " + nick + " " + command + " :Unknown command";
			break;
		case ERR_NOTEXTTOSEND:
			reply = ":" + serverName + " 412 " + nick + " :No text to send";
			break;
		case ERR_NORECIPIENT:
			reply = ":" + serverName + " 411 " + nick + " :No recipient given";
			break;
		case ERR_NOTOPLEVEL:
			reply = ":" + serverName + " 413 " + nick + " :No toplevel domain specified";
			break;
		case ERR_CANNOTSENDTOCHAN:
			reply = ":" + serverName + " 404 " + nick + " " + target + " :Cannot send to channel";
			break;
		case ERR_TOOMANYCHANNELS:
			reply = ":" + serverName + " 405 " + nick + " " + target + " :You have joined too many channels";
			break;
		case ERR_UNKNOWNMODE:
			reply = ":" + serverName + " 472 " + nick + " " + target + " :Unknown mode";
			break;
		case ERR_CHANOPRIVSNEEDED:
			reply = ":" + serverName + " 482 " + nick + " " + target + " :You're not channel operator";
			break;
		case ERR_USERONCHANNEL:
			reply = ":" + serverName + " 443 " + nick + " " + target + " :is already on channel";
			break;
		case ERR_NOSUCHNICK:
			reply = ":" + serverName + " 401 " + nick + " " + target + " :No such nick/channel";
			break;
		case ERR_NOSUCHCHANNEL:
			reply = ":" + serverName + " 403 " + nick + " " + target + " :No such channel";
			break;
		default:
			reply = ":" + serverName + " 400 " + nick + " :Unknown error";
			break;
	}
	client.sendMessage(reply + "\r\n");
}
