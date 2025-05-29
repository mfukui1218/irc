#include "Sample.hpp"
#include <functional>
#include <map>

#include <signal.h>
#include <unistd.h>

Sample::Sample()
{}

Sample::Sample(const Sample &other)
    : ATestList(other)
{
    *this = other;
}

Sample::~Sample()
{
}

Sample &Sample::operator=(const Sample &rhs)
{
    if (this != &rhs) {
        ATestList::operator=(rhs);
    }
    return *this;
}

void Sample::addAllTests()
{
    typedef std::map<std::string, TestCase::testFunc_t > testCaseMap_t;
    testCaseMap_t tests;

    tests["Sample test"] = &Sample::sampleTest;

    testCaseMap_t::iterator ite = tests.end();
    for (testCaseMap_t::iterator it = tests.begin()
        ; it != ite
        ; it++)
    {
        addTest(it->first, it->second);
    }
}

int Sample::sampleTest()
{
    return 0;
}
