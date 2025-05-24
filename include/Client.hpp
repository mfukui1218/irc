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
#include "IClient.hpp"
#include "ClientState.hpp"

class Server;
class Channel;
class Command;

class Client : public IClient {
public:
	Client(int fd);
	~Client();

	// Connection
	void connect(const std::string& address, int port);
	void disconnect();
	std::string receiveMessage();
	void sendMessage(const std::string& message);

	// Identification
	int getFd() const { return _clientFd; }
	std::string getNickname() const { return _state.getNickname(); }
	void setNickname(const std::string& nickname) { _state.setNickname(nickname); }
	std::string getUsername() const { return _state.getUsername(); }
	void setUsername(const std::string& username) { _state.setUsername(username); }
	std::string getRealname() const { return _state.getRealname(); }
	void setRealname(const std::string& realname) { _state.setRealname(realname); }
	bool isAuthenticated() const { return _state.isAuthenticated(); }
	std::string getFullIdentifier() const;
	bool isPassAccepted() const { return _state.isPassAccepted(); }

	// registered
	bool isNicknameRegistered() const { return _state.isNicknameRegistered(); }
	bool isUserRegistered() const { return _state.isUserRegistered(); }
	void setNicknameRegistered(bool registered) { _state.setNicknameRegistered(registered); }
	void setUserRegistered(bool registered) { _state.setUserRegistered(registered); }
	void setPassAccepted(bool accepted) { _state.setPassAccepted(accepted); }
	void setAuthenticated(bool auth) { _state.setAuthenticated(auth); }
	void setRecvBuffer(const std::string& buffer) { _recvBuffer = buffer; }
	std::string getRecvBuffer() const { return _recvBuffer; }

	// Channel membership
	void addJoinedChannel(const std::string& channelName) {
		_state.addJoinedChannel(channelName);
	}
	void removeJoinedChannel(const std::string& channelName) {
		_state.removeJoinedChannel(channelName);
	}
	const std::set<std::string>& getJoinedChannels() const {
		return _state.getJoinedChannels();
	}

	//only Operator control
	bool isOperatorOf(const std::string& channelName) const {
		return _state.isOperatorOf(channelName);
	}

	void addOperatorChannel(const std::string& channelName) {
		_state.grantOperator(channelName);
	}
	void removeOperatorChannel(const std::string& channelName) {
		_state.revokeOperator(channelName);
	}
	const std::set<std::string>& getOperatorChannels() const {
		return _state.getOperatorChannels();
	}
	void grantOperator(const std::string& channelName) {
		_state.grantOperator(channelName);
	}
	void revokeOperator(const std::string& channelName) {
		_state.revokeOperator(channelName);
	}

private:
	int _clientFd;
	ClientState _state;
	std::string _recvBuffer; 
};

#endif
