#include "Server.hpp"
#include <functional>
#include <algorithm>

extern volatile sig_atomic_t g_stop;

Server::Server(int port, const std::string& password) : port(port), password(password)
{
	if (port <= 0 || port > 65535)
	{
		throw std::invalid_argument("Port must be between 1 and 65535");
	}
	if (password.empty())
	{
		throw std::invalid_argument("Password cannot be empty");
	}
}

Server::~Server()
{
	stop();
}

void Server::start()
{
	std::cout << "Server started on port " << port << " with password: " << password << std::endl;
	setupSocket();
	runLoop();
	close(server_fd);
}

void Server::setupSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw std::runtime_error("Failed to create socket");
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("Bind failed");
	if (listen(server_fd, SOMAXCONN) == -1)
		throw std::runtime_error("Listen failed");
}

void Server::runLoop()
{
	_pollFds.clear();
	struct pollfd serverPoll = {server_fd, POLLIN, 0};
	_pollFds.push_back(serverPoll);
	struct pollfd pipePoll = {pipefd[0], POLLIN, 0};
	_pollFds.push_back(pipePoll);
	while (!g_stop)
	{
		int ready = poll(&_pollFds[0], _pollFds.size(), -1);
		if (ready < 0)
			throw std::runtime_error("Poll failed");

		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == server_fd)
				{
					handleNewConnection(_pollFds);
				}
				else if (_pollFds[i].fd == pipefd[0])
				{
					char buf[1];
					read(pipefd[0], buf, 1);
					std::cout << "Received shutdown signal." << std::endl;
					g_stop = 1; // Set the stop flag to true
					break;
				}
				else
				{
					if (!handleClientData(_pollFds, i))
					{
						cleanupClient(_pollFds, i);
						--i;
					}
				}
			}
		}
	}
	shutdown();
}

void Server::shutdown()
{
	std::cout << "Shutting down server..." << std::endl;

	// クライアント接続を全て閉じる
	for (size_t i = 0; i < _pollFds.size(); ++i) {
		if (_pollFds[i].fd >= 0) {
			close(_pollFds[i].fd);
		}
	}

	_pollFds.clear();

	// 全クライアントを解放
	for (size_t i = 0; i < _clients.size(); ++i) {
		delete _clients[i];
	}
	_clients.clear();

	// チャンネルも解放（もし動的確保しているなら）
	for (size_t i = 0; i < _channels.size(); ++i) {
		delete _channels[i];
	}
	_channels.clear();

	// サーバソケットも閉じる
	if (server_fd >= 0) {
		close(server_fd);
		server_fd = -1;
	}

	std::cout << "Server shut down gracefully." << std::endl;
}


void Server::handleNewConnection(std::vector<struct pollfd> &fds)
{
	client_len = sizeof(client_addr);
	int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd >= 0)
	{
		std::cout << "New client connected: fd=" << client_fd << std::endl;
		const char* welcome = "Please enter password.\n";
		send(client_fd, welcome, strlen(welcome), 0);
		struct pollfd clientPoll = { client_fd, POLLIN, 0 };
		fds.push_back(clientPoll);
		Client* newClient = new Client(client_fd);
		_clients.push_back(newClient);
	}
}

bool Server::handleClientData(std::vector<struct pollfd> &fds, size_t index)
{
	char buffer[1024];
	int bytes_received = recv(fds[index].fd, buffer, sizeof(buffer), 0);
	if (bytes_received <= 0)
		return false;

	Client* client = findClientByFd(fds[index].fd);
	if (!client)
		return false;
	std::string temp = client->getRecvBuffer();
	std::string& recvBuffer = temp;
	recvBuffer.append(buffer, bytes_received);

	size_t pos;
	while ((pos = recvBuffer.find('\n')) != std::string::npos) {
		std::string line = recvBuffer.substr(0, pos);
		recvBuffer.erase(0, pos + 1);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		std::cout << "Received command: " << line << std::endl;
		Command cmd;
		cmd.execute(*client, *this, line);
	}
	return true;
}

void Server::cleanupClient(std::vector<struct pollfd> &fds, size_t index)
{
	std::cout << "Client disconnected (fd=" << fds[index].fd << ")" << std::endl;
	Client *client = findClientByFd(fds[index].fd);
	if (client != NULL)
	{
		std::for_each(_channels.begin(), _channels.end()
			, std::bind2nd(std::mem_fun(&Channel::removeClient), client));
		std::vector<Client *>::iterator removeIt = std::find(_clients.begin(), _clients.end(), client);
		if (removeIt != _clients.end()) 
			_clients.erase(removeIt);
		delete client;
	}
	close(fds[index].fd);
	fds.erase(fds.begin() + index);
}

void Server::stop()
{
	if (server_fd >= 0)
	{
		close(server_fd);
		server_fd = -1;
	}
}

bool Server::isNicknameInUse(const std::string& nickname) const
{
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return true;
	}
	return false;
}

void Server::joinChannel(Client& client, const std::string& channelName)
{
	Channel* channel = findChannel(channelName);
	if (!channel) {
        channel = new Channel(channelName);
        _channels.push_back(channel);
        channel->addOperator(client);
    }
	if (!channel->hasClient(&client)) {
        channel->addClient(&client);
    }
	std::cout << "Client " << client.getNickname() << " joined channel: " << channelName << std::endl;
}

Channel* Server::findChannel(const std::string& channelName)
{
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel* channel = *it;
		if (channel->getChannelName() == channelName)
			return channel;
	}
	return NULL;
}

Client* Server::findClientByNickname(const std::string& nickname)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* client = *it;
		if (client->getNickname() == nickname)
			return client;
	}
	return NULL;
}

Client *Server::findClientByFd(int fd)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* client = *it;
		if (client->getFd() == fd)
			return client;
	}
	return NULL;
}
