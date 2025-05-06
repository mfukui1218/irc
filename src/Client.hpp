#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <set>
#include <sstream>
#include "Server.hpp"

class Client
{
public:
	void connect(const std::string& address, int port);
	void disconnect();
	void sendMessage(const std::string& message);
	std::string receiveMessage();
	bool isnicknameRegistered() const { return nickname_registered; }
	bool isUserRegistered() const { return user_registered; }
	bool setUserRegistered() const { return user_registered && nickname_registered; }
	void setNicknameRegistered(bool registered) { nickname_registered = registered; }
	void setUserRegistered(bool registered) { user_registered = registered; }
	void setAuthenticated(bool authenticated) { user_registered = authenticated; }
	void setNickname(const std::string& nickname) { this->nickname = nickname; }
	void setUsername(const std::string& username) { this->username = username; }
	void setRealname(const std::string& realname) { this->realname = realname; }
	std::string getNickname() const { return nickname; }
	std::string getUsername() const { return username; }
	std::string getRealname() const { return realname; }
	void joinChannel(const std::string& channelName);
	void addJoinedChannel(const std::string& channelName) { _joinedChannels.insert(channelName); }
	int getClientFd() const { return _clientFd; }
	void setClientFd(int fd) { _clientFd = fd; }
	bool isAuthenticated() const { return user_registered && nickname_registered; }

private:
	int _clientFd;
    std::string nickname;
    std::string username;
    std::string realname; 
	bool user_registered;
	bool nickname_registered;
	std::set<std::string> _joinedChannels;
};

#endif
