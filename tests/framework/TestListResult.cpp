#include "TestListResult.hpp"

TestListResult::TestListResult()
{
}

TestListResult::TestListResult(const TestListResult &other)
{
    *this = other;
}

TestListResult::~TestListResult()
{
}

TestListResult &TestListResult::operator=(const TestListResult &rhs)
{
    if (this != &rhs) {
        _testlist = rhs._testlist;
    }
    return *this;
}

TestListResult::TestListResult(size_t size)
    : _testlist(size)
{}

void TestListResult::add(const TestResult& result)
{
    _testlist.push_back(result);
}

std::vector<TestResult>::iterator TestListResult::begin()
{
    return _testlist.begin();
}

std::vector<TestResult>::iterator TestListResult::end()
{
    return _testlist.end();
}
