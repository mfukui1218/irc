#ifndef ICHANNEL_HPP
#define ICHANNEL_HPP

#include <string>
#include <set>

class Client;

class IChannel {
public:
	virtual ~IChannel() {}

	virtual std::string getName() const = 0;
	virtual void addClient(Client* client) = 0;
	virtual void removeClient(Client* client) = 0;
	virtual bool hasClient(Client* client) const = 0;
	virtual void broadcast(const std::string& message, Client* except = NULL) = 0;

	virtual void addOperator(Client& client) = 0;
	virtual void removeOperator(Client& client) = 0;
	virtual bool isOperator(Client* client) const = 0;

	virtual void setUserLimit(int limit) = 0;
	virtual void clearUserLimit() = 0;
	virtual bool isLimitEnabled() const = 0;
	virtual int getUserLimit() const = 0;
	virtual int getUserCount() const = 0;

	virtual void setInviteOnly(bool enabled) = 0;
	virtual bool isInviteOnly() const = 0;

	virtual void setTopicLock(bool enabled) = 0;
	virtual bool isTopicLocked() const = 0;
	virtual void setTopic(const std::string& topic) = 0;
	virtual std::string getTopic() const = 0;
};

#endif
