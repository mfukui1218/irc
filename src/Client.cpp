#include "Client.hpp"

Client::Client(int fd) : _clientFd(fd), _state() 
{
	_clientFd = fd;
	_state.setPassAccepted(false);
	_state.setAuthenticated(false);
	_state.setNicknameRegistered(false);
	_state.setUserRegistered(false);
}

Client::~Client() {
	if (_clientFd >= 0) {
		close(_clientFd);
	}
}

void Client::connect(const std::string& address, int port)
{
	_clientFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_clientFd < 0)
		throw std::runtime_error("Failed to create socket");
	struct sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr) <= 0)
		throw std::runtime_error("Invalid address");
	if (::connect(_clientFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		throw std::runtime_error("Connection failed");
	std::cout << "Connected to " << address << ":" << port << std::endl;
}

void Client::disconnect()
{
	if (_clientFd >= 0)
	{
		close(_clientFd);
		_clientFd = -1;
	}
}

void Client::sendMessage(const std::string& message)
{
	if (_clientFd < 0)
		throw std::runtime_error("Not connected");
	ssize_t bytesSent = send(_clientFd, message.c_str(), message.size(), 0);
	if (bytesSent < 0)
		throw std::runtime_error("Failed to send message");
}

std::string Client::receiveMessage()
{
	if (_clientFd < 0)
		throw std::runtime_error("Not connected");
	char buffer[1024];
	ssize_t bytesReceived = recv(_clientFd, buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived < 0)
		throw std::runtime_error("Failed to receive message");
	buffer[bytesReceived] = '\0';
	return std::string(buffer);
}

std::string Client::getFullIdentifier() const {
	return getNickname() + "!" + getUsername() + "@localhost";
}
