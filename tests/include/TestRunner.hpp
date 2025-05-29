#ifndef TEST_RUNNER_HPP
# define TEST_RUNNER_HPP

# include "ATestList.hpp"
# include "TestResult.hpp"
# include "TestListResult.hpp"
# include <string>

class TestRunner
{
    private:
    protected:
    public:
        TestRunner();
        TestRunner(const TestRunner &other);
        virtual ~TestRunner();
        TestRunner &operator=(const TestRunner &rhs);
        TestListResult runTestList(ATestList &test);
        TestResult runTest(const TestCase &testCase) const;
};

#endif // TEST_RUNNER_HPP
