#include "ClientState.hpp"

ClientState::ClientState()
	: _nickname(""), _username(""), _realname(""),
	  _nicknameRegistered(false), _userRegistered(false) {}
bool ClientState::isNicknameRegistered() const {
	return _nicknameRegistered;
}

bool ClientState::isUserRegistered() const {
	return _userRegistered;
}

void ClientState::setNicknameRegistered(bool registered) {
	_nicknameRegistered = registered;
}

void ClientState::setUserRegistered(bool registered) {
	_userRegistered = registered;
}

bool ClientState::isAuthenticated() const {
	return _nicknameRegistered && _userRegistered && _passAccepted;
}

std::string ClientState::getNickname() const {
	return _nickname;
}

void ClientState::setNickname(const std::string& nick) {
	_nickname = nick;
}

std::string ClientState::getUsername() const {
	return _username;
}

void ClientState::setUsername(const std::string& user) {
	_username = user;
}

std::string ClientState::getRealname() const {
	return _realname;
}

void ClientState::setRealname(const std::string& realname) {
	_realname = realname;
}

void ClientState::addJoinedChannel(const std::string& channelName) {
	_joinedChannels.insert(channelName);
}

const std::set<std::string>& ClientState::getJoinedChannels() const {
	return _joinedChannels;
}

bool ClientState::isOperatorOf(const std::string& channelName) const {
	return _operatorChannels.find(channelName) != _operatorChannels.end();
}

void ClientState::grantOperator(const std::string& channelName) {
	_operatorChannels.insert(channelName);
}

void ClientState::revokeOperator(const std::string& channelName) {
	_operatorChannels.erase(channelName);
}

const std::set<std::string>& ClientState::getOperatorChannels() const {
	return _operatorChannels;
}

void ClientState::removeJoinedChannel(const std::string& channelName) {
	_joinedChannels.erase(channelName);
}

bool ClientState::isPassAccepted() const {
	return _passAccepted;
}

void ClientState::setPassAccepted(bool accepted) {
	_passAccepted = accepted;
}

void ClientState::setAuthenticated(bool auth) {
	_authenticated = auth;
}
