#ifndef TEST_LAUNCHER_HPP
# define TEST_LAUNCHER_HPP

# include "ATestList.hpp"

class TestLauncher
{
    public:
        TestLauncher();
        TestLauncher(const TestLauncher &other);
        virtual ~TestLauncher();
        TestLauncher operator=(const TestLauncher &rhs);
        template <typename T> void launch();
};

# include "TestLauncher.tpp"

#endif // TEST_LAUNCHER_HPP
