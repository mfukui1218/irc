#ifndef COMMAND_SAMPLE_HPP
# define COMMAND_SAMPLE_HPP

#include "ICommand.hpp"

class CommandSample : public ICommand
{
	private:
		std::string channel_etc;
		CommandSample();

	public:
		CommandSample(const CommandSample &other);
		~CommandSample();
		CommandSample operator=(const CommandSample &rhs);

		CommandSample(std::string args); // コマンド実行に必要な諸々の初期設定を渡す。
		void execute(Session *session, IRCServer *server); // 引数を渡して実行する。（実際の実行処理をここに記載する）
};

#endif // COMMAND_SAMPLE_HPP
