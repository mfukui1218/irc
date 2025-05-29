#ifndef ATEST_LIST_HPP
# define ATEST_LIST_HPP

# include "TestCase.hpp"
# include <string>
# include <vector>

class TestCase;

class ATestList
{
    private:
        std::string _caseName;
        std::vector <TestCase> _testList;

    public:
        typedef std::vector<TestCase>::iterator iterator;
        typedef std::vector<TestCase>::const_iterator const_iterator;

        ATestList(const std::string &caseName);
        ATestList &operator=(const ATestList &rhs);
        ATestList(const ATestList &other);
        ATestList();
        virtual ~ATestList();
        virtual void preprocess();
        virtual void postprocess();
        virtual void addAllTests() = 0;
        const_iterator begin() const;
        const_iterator end() const;

    protected:
        void addTest(const std::string& caseName, TestCase::testFunc_t fun);
};

#endif // ATEST_LIST_HPP
