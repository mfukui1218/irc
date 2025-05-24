#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"

class Client;  // Forward declaration
class Channel; // Forward declaration

class Server {
public:
	Server(int port, const std::string& password);
	~Server();

	// Server control
	void start();
	void stop();

	// Password and info
	std::string getPassword() const { return password; }
	bool isNicknameInUse(const std::string& nickname) const;

	// Client and channel management
	void joinChannel(Client& client, const std::string& channelName);
	Channel* findChannel(const std::string& channelName);
	void addChannel(Channel* channel) { _channels.push_back(channel); }
	Client* findClientByNickname(const std::string& nickname);
	Client* findClientByFd(int fd);

private:
	// Socket setup and main loop
	void setupSocket();
	void runLoop();
	void handleNewConnection(std::vector<struct pollfd>& fds);
	bool handleClientData(std::vector<struct pollfd>& fds, size_t index);
	void cleanupClient(std::vector<struct pollfd>& fds, size_t index);

	// Core server members
	int port;
	std::string password;
	int server_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t client_len;
	std::vector<Client*> _clients;
	std::vector<struct pollfd> _pollFds;
	std::vector<Channel*> _channels;
};

#endif
