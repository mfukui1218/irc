#ifndef ANUMERIC_REPLY_HPP
#define ANUMERIC_REPLY_HPP

#include "Host.hpp"
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
        const Host *_server;
        std::string _msg;

    protected:
        // 外からインスタンス化させないためにprotected
        ANumericReply();
        ANumericReply(const ANumericReply &other);
        ANumericReply operator=(const ANumericReply &rhs);
        ANumericReply(int num, const Host &server, const std::string &msg);

    public:
        virtual ~ANumericReply();

        std::string getText();
};

#endif // ANUMERIC_REPLY_HPP
