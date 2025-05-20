#include "ANumericReply.hpp"
#include <sstream>

ANumericReply::ANumericReply() {}

ANumericReply::ANumericReply(const ANumericReply &other)
{
    *this = other;
}

ANumericReply::~ANumericReply() {}

ANumericReply ANumericReply::operator=(const ANumericReply &rhs)
{
    if (this != &rhs)
    {
        _num = rhs._num;
        _serverName = rhs._serverName;
        _receiverName = rhs._receiverName;
        _msg = rhs._msg;
    }
    return *this;
}
ANumericReply::ANumericReply(const std::string &serverName,
                             int num,
                             const std::string &receiverName,
                             const std::string &msg)
    : _num(num)
    , _serverName(serverName)
    , _receiverName(receiverName)
    , _msg(msg)
{}

int ANumericReply::getNum(void) const
{
    return _num;
}

const std::string &ANumericReply::getServerName(void) const
{
    return _serverName;
}

const std::string &ANumericReply::getReceiverName(void) const
{
    return _receiverName;
}

const std::string &ANumericReply::getMsg(void) const
{
    return _msg;
}

void ANumericReply::setMsg(const std::string &msg)
{
    _msg = msg;
}

/// @brief make message
/// @return `string` with this format: `:serverName num receiverName message`.
std::string ANumericReply::getFullMessage() const
{
    std::ostringstream oss;

    oss << ":" << getServerName() << " ";
    oss << getNum() << " ";
    oss << getReceiverName() << " ";
    oss << getMsg() << std::endl;
    return oss.str();
}
