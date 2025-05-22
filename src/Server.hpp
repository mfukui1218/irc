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

class Channel; // Forward declaration

class Server
{
public:
    Server(int port, const std::string& password);
    ~Server();

    void start();
    void stop();
    std::string getPassword() const { return password; }
    bool isNicknameInUse(const std::string& nickname) const;
    void joinChannel(Client& client, const std::string& channelName);
    Channel* findChannel(const std::string& channelName);
    void handleJoin(Client& client, const std::string& args);
    void addChannel(Channel* channel) { _channels.push_back(channel); }
    Client *findClientByNickname(const std::string& nickname);
    Client *findClientByFd(int fd);

private:
    void setupSocket();
    void runLoop();
    void handleNewConnection(std::vector<struct pollfd>& fds);
    bool handleClientData(std::vector<struct pollfd> &fds, size_t index);
    void cleanupClient(std::vector<struct pollfd> &fds, size_t index);

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
