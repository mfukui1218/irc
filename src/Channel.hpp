#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include "Client.hpp"

class Client;  // Forward declaration

class Channel 
{
private:
    std::string channelName;
    std::string topic;
    std::set<Client*> _clients;
    std::set<Client*> operators;
    std::string key;  // +k mode (password)
    int userLimit;    // +l mode
    bool inviteOnly;  // +i mode

public:
    Channel(const std::string& name);
    ~Channel();

    void addClient(Client* client);
    void removeClient(Client* client);
    bool hasClient(Client* client) const;

    void addOperator(Client* client);
    void removeOperator(Client* client);
    bool isOperator(Client* client) const;

    void setTopic(const std::string& topic);
    std::string getTopic() const;

    void setKey(const std::string& key);
    std::string getKey() const;

    void setUserLimit(int limit);
    int getUserLimit() const;

    void setInviteOnly(bool invite);
    bool isInviteOnly() const;

    std::string getChannelName() const;
	void broadcast(const std::string& message);

	std::set<Client*> getClients() const
	{
		return _clients;
	}

};

#endif
