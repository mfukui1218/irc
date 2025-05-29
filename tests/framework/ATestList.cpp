#include "ATestList.hpp"

ATestList::ATestList()
{
}

ATestList::ATestList(const ATestList &other)
{
    *this = other;
}

ATestList::~ATestList()
{}

ATestList &ATestList::operator=(const ATestList &rhs)
{
    if (this != &rhs) {
        _caseName = rhs._caseName;
        _testList = rhs._testList;
    }
    return *this;
}

ATestList::ATestList(const std::string &caseName)
    :_caseName(caseName)
{}

void ATestList::preprocess()
{}

void ATestList::postprocess()
{}

void ATestList::addTest(const std::string &caseName, TestCase::testFunc_t func)
{
    _testList.push_back(TestCase(caseName, func));
}

ATestList::const_iterator ATestList::begin() const
{
    return _testList.begin();
}

ATestList::const_iterator ATestList::end() const
{
    return _testList.end();
}
