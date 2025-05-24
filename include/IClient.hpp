#ifndef ICLIENT_HPP
#define ICLIENT_HPP

#include <string>

class Server;

class IClient {
public:
	virtual ~IClient() {}

	virtual int getFd() const = 0;
	virtual std::string getNickname() const = 0;
	virtual void setNickname(const std::string&) = 0;
	virtual std::string getUsername() const = 0;
	virtual void setUsername(const std::string&) = 0;
	virtual std::string getRealname() const = 0;
	virtual void setRealname(const std::string&) = 0;
	virtual bool isAuthenticated() const = 0;
	virtual void setAuthenticated(bool) = 0;
	virtual void addJoinedChannel(const std::string&) = 0;
	virtual const std::set<std::string>& getJoinedChannels() const = 0;
	virtual void addOperatorChannel(const std::string&) = 0;
	virtual const std::set<std::string>& getOperatorChannels() const = 0;
	virtual std::string getFullIdentifier() const = 0;
	virtual void removeOperatorChannel(const std::string&) = 0;
	virtual void sendMessage(const std::string&) = 0;
	virtual bool isOperatorOf(const std::string& channelName) const = 0;

	//only operator can call this
	virtual void grantOperator(const std::string& channelName) = 0;
	virtual void revokeOperator(const std::string& channelName) = 0;
	virtual void removeJoinedChannel(const std::string&) = 0;
};


#endif
