#ifndef TEST_LIST_RESULT_HPP
# define TEST_LIST_RESULT_HPP

# include "TestResult.hpp"
# include <vector>

class TestListResult
{
    private:
        std::vector<TestResult> _testlist;

    public:
        TestListResult();
        TestListResult(const TestListResult &other);
        virtual ~TestListResult();
        TestListResult &operator=(const TestListResult &rhs);
        explicit TestListResult(size_t size);

        void add(const TestResult& result);
        std::vector<TestResult>::iterator begin();
        std::vector<TestResult>::iterator end();

        class iterator : public std::vector<TestResult> {};
};

#endif // TEST_LIST_RESULT_HPP
