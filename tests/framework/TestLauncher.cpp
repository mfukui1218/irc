#include "TestLauncher.hpp"
#include "TestRunner.hpp"

TestLauncher::TestLauncher()
{
}

TestLauncher::TestLauncher(const TestLauncher &other)
{
    *this = other;
}

TestLauncher::~TestLauncher()
{
}

TestLauncher TestLauncher::operator=(const TestLauncher &rhs)
{
    if (this != &rhs) {
        // assignment logic
    }
    return *this;
}
