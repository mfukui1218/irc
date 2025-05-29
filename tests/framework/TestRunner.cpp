#include "TestRunner.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <functional>

extern "C" {
    #include <unistd.h>
    #include <sys/wait.h>
}

TestRunner::TestRunner()
{
}

TestRunner::TestRunner(const TestRunner &other)
{
    *this = other;
}

TestRunner::~TestRunner()
{
}

TestRunner &TestRunner::operator=(const TestRunner &rhs)
{
    if (this != &rhs) {
        // assignment logic
    }
    return *this;
}

TestListResult TestRunner::runTestList(ATestList &test)
{
    TestListResult results;
    try {
        test.preprocess();
    } catch (const std::exception &e) {
        std::cout << "Fail preprocess: " << e.what();
        return (results);
    }
    ATestList::const_iterator ite = test.end();
    for (ATestList::const_iterator it = test.begin(); it != ite; it++)
        results.add(runTest(*it));
    try {
        test.postprocess();
    } catch (const std::exception &e) {
        std::cout << "Fail postprocess: " << e.what();
        return (results);
    }
    return (results);
}

TestResult TestRunner::runTest(const TestCase &testCase) const
{
    TestResult result(testCase.getName());
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        result.setType(TestResult::INTERNAL);
        return result;
    }
    else if (pid == 0)
    {
        alarm(3);
        exit(testCase.run());
    }
    int wstat;
    wait(&wstat);
    if (WIFEXITED(wstat))
    {
        if (WEXITSTATUS(wstat) == 0)
            result.setType(TestResult::OK);
        else
            result.setType(TestResult::NG);
    }
    else if (WIFSIGNALED(wstat))
    {
        result.setType(TestResult::SIGNAL);
        result.setSignal(WTERMSIG(wstat));
    }
    else
        result.setType(TestResult::INTERNAL);
    result.print();
    return result;
}
