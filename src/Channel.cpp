#include "Channel.hpp"

Channel::Channel(const std::string& name) : channelName(name), userLimit(0), inviteOnly(false) {}

Channel::~Channel() {}

void Channel::addClient(Client* client) {
	_clients.insert(client);
}

void Channel::removeClient(Client* client) {
	_clients.erase(client);
}

bool Channel::hasClient(Client* client) const {
	return _clients.find(client) !=_clients.end();
}

void Channel::addOperator(Client* client) {
	operators.insert(client);
}

void Channel::removeOperator(Client* client) {
	operators.erase(client);
}

bool Channel::isOperator(Client* client) const {
	return operators.find(client) != operators.end();
}

void Channel::setTopic(const std::string& topic) {
	this->topic = topic;
}

std::string Channel::getTopic() const {
	return topic;
}

void Channel::setKey(const std::string& key) {
	this->key = key;
}

std::string Channel::getKey() const {
	return key;
}

void Channel::setUserLimit(int limit) {
	this->userLimit = limit;
}

int Channel::getUserLimit() const {
	return userLimit;
}

void Channel::setInviteOnly(bool invite) {
	this->inviteOnly = invite;
}

bool Channel::isInviteOnly() const {
	return inviteOnly;
}

std::string Channel::getChannelName() const {
	return channelName;
}

void Channel::broadcast(const std::string& message) {
	for (std::set<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		Client* client = *it;
		client->sendMessage(message);
	}
}
