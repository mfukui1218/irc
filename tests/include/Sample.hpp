#ifndef SAMPLE_HPP
# define SAMPLE_HPP

# include "ATestList.hpp"

class Sample : public ATestList
{
    private:
    protected:
    public:
        Sample();
        Sample(const Sample &other);
        virtual ~Sample();
        Sample &operator=(const Sample &rhs);
        void addAllTests();

        // tests
        static int sampleTest();
};

#endif // SAMPLE_HPP
