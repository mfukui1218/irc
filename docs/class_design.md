
# クラス図を作成に使用したMermaidについて
 * https://zenn.dev/tak_uchida/articles/da583cf960e854

# 大体の動きのイメージ
```cpp
main(int argc, char *argv[])
{
	if (argc != 3)
		return (1); // エラーメッセージ

	try {
		Config config(argv[1], argv[2]); // port, password, etc...
		SessionManager sessionManager();
		ChannelManager channelManager();
		IRCServer server(config, channelManager, sessionManager); // ポリモーフィズムのために参照渡しすること。
	
		IRCObserver observer();
		observer.addEventListener(server);
		observer.startObserve(); // ここに入ったらもうエラーでは終了しないこと。
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
```

# 全体
```mermaid
classDiagram

	IRCServer <-- IChannelManager
	IRCServer <-- IClientManager
	IRCServer <-- IRCConfig
	IRCServer <-- IMessageProxy

	IRCServer <|.. IEventListener

	class IRCServer {
		 + getChannelManager()
		 + getClientManager()
		 + getConfig()
		 + getMessageProxy()
		 + onConnected()
		 + onCommunication()
		 + onDisconnected()
	}

	class IRCObserver {
		- eventListenerList[]$
		+ getInstance()$
		+ destroyInstance()$

		+ addEventListener()
		+ removeEventListener()
		+ startObserve()
		+ stopObserve()
	}

	class IEventListener {
		<<interface>>
		 + onConnected()
		 + onCommunication()
		 + onDisconnected()
	}
	
```

# IRCConfig - 設定
```mermaid
classDiagram
	class IRCConfig {
		- _port
		- _password
	}
```

# IClientManager - クライアントマネージャー
```mermaid
classDiagram
	IClient <-- IClientManager
	class IClientManager {
		<<interface>>
		 + registerClient()
		 + findClient()
		 + unregisterClient()
	}
```

# IClient - クライアント
サーバー・クライアント
```mermaid
classDiagram
	IClient <|-- Client
	IClient <|-- Operator
	IClient <-- IClientManager

	class IClient {
		<<interface>>
		+ getNickName()
	}

	class Operator {
		- _nickName
		+ getNickName()
		+ onConnected()
		+ onCommunication()
		+ onDisconnected()
	}

	class Client {
		- _nickName
		+ getNickName()
		+ onConnected()
		+ onCommunication()
		+ onDisconnected()
	}
	
```

# IEventListener - イベント処理
```mermaid
classDiagram

	IEventListener <-- IRCObserver
	class IEventListener {
		<<interface>>
		+ onConnected()
		+ onCommunication()
		+ onDisconnected()
	}
```

# Session - セッション
ソケット通信における接続を以下のようにセッションとして定義する。
```mermaid
classDiagram
	class Session {
		- _client
		- _pollfd
		+ createStringStream()
	}

	Session <-- ISessionManager
	class ISessionManager {
		+ addSession()
		+ removeSession()
		+ findSessions()
	}
```

# Host - ホスト
```mermaid
classDiagram
	class Host {
		+ getHostName() : わからないけどたぶん必要
	}
```

# IChannel - チャンネル
チャンネル
```mermaid
classDiagram
	ChannelManager --> IChannel
	IChannel <|-- LocalChannel
	IChannel <|-- GlobalChannel

	class IChannel {
		<<interface>>
		+ canSendMessage()
		+ join(client)
	}

	class IChannelManager {
		<<interface>>
		+ registerChannel()
		+ findChannel()
		+ unregisterChannel()
	}
```

# ICommand - コマンド
コマンドに関する処理
```mermaid
classDiagram
	direction TB
	ICommand <|.. NickCommand
	ICommand <|.. UserCommand
	ICommand <|.. PassCommand
	ICommand <|.. JoinCommand
	ICommand <|.. KickCommand
	ICommand <|.. InviteCommand
	ICommand <|.. TopicCommand
	ICommand <|.. ModeCommand
	ICommand <|.. PrivateMessageCommand
	ICommand <|.. ErrorCommand

	class ICommand {
		<<interface>>
		+ void execute()
	}
```

# CommandFactory  - コマンド生成
コマンド生成に関する処理。parser
```mermaid
classDiagram
	class CommandFactory {
		+ createCommand()
	}
```

# Logger
ログに関する処理
```mermaid
classDiagram
	class Logger {
		+ void error()$
		+ void warn()$
		+ void debug()$
		+ void info()$
	}
```

# ANumericReply
各リプライクラスを定義する。
これらは必要になったときに作ること。
コンストラクタで必要な情報をそれぞれ渡す。
```mermaid
classDiagram
	
	ANumericReply <|.. SomeError
	ANumericReply <|.. SomeReply

	class ANumericReply {
		<<abstract>>
		+ getText()*
	}
```

# IMessageProxy - メッセージ送信代行
リクエスト
```mermaid
classDiagram
	class IMessageProxy {
		+ sendMessage()
		+ flushMessage()
	}
```
