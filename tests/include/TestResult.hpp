#ifndef TEST_RESULT_HPP
# define TEST_RESULT_HPP

# include <string>
# include <iostream>

class TestResult
{
    public:
        static bool isColored;
        enum e_result_type {
            MIN_TYPENUM,
            NONE,
            OK,
            NG,
            SIGNAL,
            INTERNAL,
            MAX_TYPENUM,
        };

    private:

        std::string _caseName;
        e_result_type _type;
        int _signal;

    public:
        TestResult();
        TestResult(const TestResult &other);
        virtual ~TestResult();
        TestResult &operator=(const TestResult &rhs);
        TestResult(const std::string &caseName);
        void setType(e_result_type type);
        void setSignal(int signal);
        std::string getText(void) const;
        void print(std::ostream &o = std::cout) const;
};

std::ostream &operator<<(std::ostream &o, TestResult &rhs);

#endif // TEST_RESULT_HPP
