#include "IRCConfig.hpp"

/// @brief default: port "6667" password: "password"
IRCConfig::IRCConfig()
{
}

/// @brief コピーする
/// @param other 
IRCConfig::IRCConfig(const IRCConfig &other)
{
	// copy constructor
}

/// @brief デストラクタ
IRCConfig::~IRCConfig()
{
}

/// @brief 
/// @param rhs 
/// @return 
IRCConfig IRCConfig::operator=(const IRCConfig &rhs)
{
	if (this != &rhs) {
		// assignment logic
	}
	return *this;
}

/// @brief 引数の通りに初期化する
/// @param port サーバーでリッスンするポート番号
/// @param password サーバーのパスワード
IRCConfig::IRCConfig(int port, std::string password)
{
}
