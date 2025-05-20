#ifndef ANUMERIC_REPLY_HPP
#define ANUMERIC_REPLY_HPP

#include <string>

/**
 * エラーのメッセージも普通のリプライもしっかりと継承しましょう。
 * class SomeReply : public ANumericReply
 * {
 *      [...]
 *      SomeReply(const std::string &channel); //
 * 各リプライで固有のデータ・文字列を挿入します。
 *      [...]
 * }
 */
class ANumericReply
{
    private:
        int _num;
        std::string _serverName;
        std::string _receiverName;

    protected:
        // 外からインスタンス化させないためにprotected
        ANumericReply();
        ANumericReply(const ANumericReply &other);
        ANumericReply &operator=(const ANumericReply &rhs);
        ANumericReply(const std::string &serverName,
                      int num,
                      const std::string &receiverName);
        
    public:
        virtual ~ANumericReply();

        int getNum(void) const;
        const std::string &getServerName(void) const;
        const std::string &getReceiverName(void) const;
        virtual std::string getMsg(void) const = 0;
        
        std::string getFullMessage() const;
};

#endif // ANUMERIC_REPLY_HPP
