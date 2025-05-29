#include "TestLauncher.hpp"
#include "TestRunner.hpp"

template <typename T> void TestLauncher::launch()
{
    T testlist;
    TestRunner runner;

    testlist.addAllTests();
    runner.runTestList(testlist);
}
