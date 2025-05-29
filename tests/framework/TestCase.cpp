#include "TestCase.hpp"

TestCase::TestCase()
    : _fun(NULL)
{
}

TestCase::TestCase(const TestCase &other)
    : _fun(NULL)
{
    *this = other;
}

TestCase::~TestCase()
{
}

TestCase &TestCase::operator=(const TestCase &rhs)
{
    if (this != &rhs) {
        _caseName = rhs._caseName;
        _fun = rhs._fun;
    }
    return *this;
}

TestCase::TestCase(const std::string &caseName, testFunc_t fun)
    : _caseName(caseName)
    , _fun(fun)
{}

const std::string &TestCase::getName() const
{
    return _caseName;
}

int TestCase::run() const
{
    if (_fun == NULL)
        return -1;
    return (_fun)();
}
