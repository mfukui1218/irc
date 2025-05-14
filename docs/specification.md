
# 接続の登録
 `PASS`コマンドは必要ないが、以下の順序での使用がおすすめされている。
 接続に成功したら、RPL_WELCOMEに従いメッセージを出力すること。
 1. PASS
 2. NICK or SERVICE
 3. USER
> 参照：
> [Connection Registration](https://datatracker.ietf.org/doc/html/rfc2812#section-2.4:~:text=April%202000%0A%0A%0A3.1-,Connection%20Registration,-The%20commands%20described)
 
# エラーがあっても終了してはいけない。

# 応答フォーマット

```
:prefix 3digit params
```
prefix: 送信者
3digit: 3桁の応答番号
params: 応答の宛先
ここで、サーバーからの応答は、prefixがサーバー名になり、paramsは宛先（クライアント or サーバー）となる。

> 参照：
> [Message format in Augmented BNF](https://datatracker.ietf.org/doc/html/rfc2812#page-6:~:text=current%20implementations.%0A%0A2.3.1-,Message%20format%20in%20Augmented%20BNF,-The%20protocol%20messages)
> [Numeric replies](https://datatracker.ietf.org/doc/html/rfc2812#section-2.4:~:text=for%0A%20%20%20%20%20%20%20%20%20%3Cparams%3E.%0A%0A2.4-,Numeric%20replies,-Most%20of%20the)

# チャンネル
　チャンネル名には文字数制限とプリフィックス、`&`, `#`, `+`, `!`がある。（他にも要件はあるが割愛）
　各プリフィックスは以下の意味で使われる。
 * '&'：ローカルサーバーにのみ属するチャンネル
 * '#'：たぶんグローバルチャンネル（必要なし）
 * '+'：チャンネルモード非対応
 * '!'：チャンネル作成者として識別される。

## モード
 * itkolのモードが使用可能であるのでそれに対応する。
 * ユーザーのモード変更ではない。
 > 参照：
 > [Channel mode message](https://datatracker.ietf.org/doc/html/rfc2812#page-18:~:text=April%202000%0A%0A%0A3.2.3-,Channel%20mode%20message,-Command%3A%20MODE%0A%20%20%20Parameters)

## アクセス制御
 KICKコマンドが存在しているためアクセス制御を行う。
　[Channel Access Control](https://datatracker.ietf.org/doc/html/rfc2811#page-10:~:text=MODE%20query.%0A%0A4.3-,Channel%20Access%20Control,-The%20last%20category)

## 権限
 オペレータが存在している必要がある。
 これらのオペレータは、＠がつくらしい。・・・？
 > [Channel Operators](https://datatracker.ietf.org/doc/html/rfc2811#page-16:~:text=April%202000%0A%0A%0A2.4.1-,Channel%20Operators,-The%20channel%20operators)

# Ping
　IRCサーバーはPINGコマンドが定期的に送られない場合、接続を切る。
　どのタイミングで接続を切るかはpollと時間の関数を使えば実現可能そうであるが、添付ファイルのサーバーでは行っていない処理なので無視するのがよさそう。

# コマンド
　以下のコマンドに対応する。 ERR_CHANOPRIVSNEEDEDが応答の種類に指定されているときは、
権限に注意すること。
　一部のコマンドは、存在していないチャンネルがあるときはコマンド実行できないことに注意すること。
 * NICK
 * USER
 * PASS
 * JOIN
 * KICK
 * INVITE
 * TOPIC
 * MODE
 * PRIVATEMESSAGE
 * ERROR

# スペック
 * pollを使うと遅くなるがepollを使うと早くなる。（詳細はman）余裕があれば。
