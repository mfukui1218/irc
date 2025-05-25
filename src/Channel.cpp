#include "Channel.hpp"
#include "ft.hpp"

Channel::Channel(const std::string& name) : _channelName(name), _userLimit(0), _inviteOnly(false) {}
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

void Channel::addOperator(Client& client) {
	_operators.insert(&client);
}

void Channel::removeOperator(Client& client) {
	_operators.erase(&client);
}

bool Channel::isOperator(Client* client) const {
	return _operators.find(client) != _operators.end();
}

void Channel::setTopic(const std::string& topic) {
	this->_topic = topic;
}

std::string Channel::getTopic() const {
	return _topic;
}

void Channel::setKey(const std::string& key) {
	this->_key = key;
}

void Channel::removeKey() {
	this->_key.clear();
}

std::string Channel::getKey() const {
	return _key;
}

void Channel::setUserLimit(int limit) {
	this->_userLimit = limit;
}

int Channel::getUserLimit() const {
	return _userLimit;
}

void Channel::setInviteOnly(bool invite) {
	this->_inviteOnly = invite;
}

bool Channel::isInviteOnly() const {
	return _inviteOnly;
}

std::string Channel::getChannelName() const {
	return _channelName;
}

void Channel::broadcast(const std::string& message, Client* except) {

	for (std::set<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		Client* client = *it;
		if (client != except)
			client->sendMessage(message);
	}
}

bool Channel::isLimitEnabled() const {
	return _userLimit > 0;
}

int Channel::getUserCount() const {
	return _clients.size();
}

bool Channel::hasMember(const Client& client) const {
	return _clients.find(const_cast<Client*>(&client)) != _clients.end();
}

void Channel::applyMode(const std::string& modeStr, std::stringstream& argStream,  const Client &client, Server &server) {
	bool adding;
    for (size_t i = 0; i < modeStr.size(); ++i) {
        char m = modeStr[i];
        if (m == '+') adding = true;
        else if (m == '-') adding = false;
        else {
            switch (m) {
                case 'i': _inviteOnly = adding; break;
                case 't': _topicLocked = adding; break;
                case 'k':{
					std::string password;
					argStream >> password;
					set_channelpass(adding, password, client);
					break;
				}
                case 'o':{
					std::string nickname;
					argStream >> nickname;
					set_operator(adding, nickname, (Client &)client, server);
				} break;
                case 'l':{
					int limit;
					argStream >> limit;
					set_userlimit(adding, limit, client);
				} break;
            }
        }
    }
}

void Channel::removeMode(const std::string& mode) {
	if (mode == "i") _inviteOnly = false;
	else if (mode == "t") _topicLocked = false;
	else if (mode == "k") removeKey();
	else if (mode == "o") {} // No action needed for operator mode
	else if (mode == "l") setUserLimit(0);
}

void Channel::set_channelpass(bool adding, const std::string& password, const Client& client) {
	if (adding) {
		if (_key == password) {
			std::cout << "Password already set" << std::endl;
			return;
		}
		setKey(password);
		broadcast(":" + client.getFullIdentifier() + " MODE " + _channelName + " +k " + password + "\n");
	} else {
		removeKey();
		broadcast(":" + client.getFullIdentifier() + " MODE " + _channelName + " -k\n");
	}
}

void Channel::set_operator(bool adding, const std::string& nickname, Client& issuer, Server& server) {
	Client* target = server.findClientByNickname(nickname);
	if (!target) {
		issuer.sendMessage("401 " + nickname + " :No such nick\n");
		return;
	}
	if (!hasMember(*target)) {
		issuer.sendMessage("441 " + nickname + " " + _channelName + " :They aren't on that channel\n");
		return;
	}
	if (adding) {
		target->grantOperator(_channelName);
		broadcast(":" + issuer.getFullIdentifier() + " MODE " + _channelName + " +o " + nickname + "\n");
	} else {
		target->revokeOperator(_channelName);
		broadcast(":" + issuer.getFullIdentifier() + " MODE " + _channelName + " -o " + nickname + "\n");
	}
}

void Channel::set_userlimit(bool adding, int limit, const Client& client) {
	if (adding) {
		if (_userLimit == limit) {
			std::cout << "User limit already set" << std::endl;
			return;
		}
		setUserLimit(limit);
		broadcast(":" + client.getFullIdentifier() + " MODE " + _channelName + " +l " + ft::to_string(limit) + "\n");
	} else {
		setUserLimit(0);
		broadcast(":" + client.getFullIdentifier() + " MODE " + _channelName + " -l\n");
	}
}
