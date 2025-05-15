#include "ANumericReply.hpp"

/// @brief
ANumericReply::ANumericReply()
    : _num(0)
    , _server(NULL)
    , _msg("")
{}

/// @brief
/// @param other
ANumericReply::ANumericReply(const ANumericReply &other)
{
    (void)other;
    // copy constructor
}

/// @brief
ANumericReply::~ANumericReply() {}

/// @brief
/// @param rhs
/// @return
ANumericReply ANumericReply::operator=(const ANumericReply &rhs)
{
    if (this != &rhs)
    {
        // assignment logic
    }
    return *this;
}

ANumericReply::ANumericReply(int num, const Host &server, const std::string &msg)
    : _num(num)
    , _server(&server)
    , _msg(msg)
{}

std::string ANumericReply::getText() {
	return ("");
}
