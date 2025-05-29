#ifndef TEST_CASE_HPP
# define TEST_CASE_HPP

# include <string>

class TestCase
{
    public:
        typedef int (*testFunc_t)();

    private:
        std::string _caseName;
        testFunc_t _fun;

    public:
        TestCase();
        TestCase(const TestCase &other);
        TestCase &operator=(const TestCase &rhs);
        virtual ~TestCase();
        TestCase(const std::string &caseName, testFunc_t fun);
        const std::string &getName() const;
        int run() const;
};

#endif // TEST_CASE_HPP
