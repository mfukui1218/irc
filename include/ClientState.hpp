#ifndef CLIENTSTATE_HPP
#define CLIENTSTATE_HPP

#include <string>
#include <set>

class ClientState {
public:
	ClientState();

	// 認証状態の管理
	bool isNicknameRegistered() const;
	bool isUserRegistered() const;
	void setNicknameRegistered(bool registered);
	void setUserRegistered(bool registered);
	bool isAuthenticated() const;
	void setAuthenticated(bool auth);
	bool isPassAccepted() const;
	void setPassAccepted(bool accepted);


	// ニックネーム・ユーザー名・実名
	std::string getNickname() const;
	void setNickname(const std::string& nick);

	std::string getUsername() const;
	void setUsername(const std::string& user);

	std::string getRealname() const;
	void setRealname(const std::string& realname);

	// チャンネル情報
	void addJoinedChannel(const std::string& channelName);
	const std::set<std::string>& getJoinedChannels() const;
	void removeJoinedChannel(const std::string& channelName);

	bool isOperatorOf(const std::string& channelName) const;
	void grantOperator(const std::string& channelName);
	void revokeOperator(const std::string& channelName);
	const std::set<std::string>& getOperatorChannels() const;

private:
	// 基本情報
	std::string _nickname;
	std::string _username;
	std::string _realname;

	// 認証状態
	bool _nicknameRegistered;
	bool _userRegistered;
	bool _authenticated;
	bool _passAccepted;

	// チャンネル
	std::set<std::string> _joinedChannels;
	std::set<std::string> _operatorChannels;
};

#endif
