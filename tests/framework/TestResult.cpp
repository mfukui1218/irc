#include "TestResult.hpp"
#include <map>

#include <signal.h>

bool TestResult::isColored = true;

TestResult::TestResult()
    : _type(NONE)
    , _signal(0)
{}

TestResult::TestResult(const TestResult &other)
{
    *this = other;
}

TestResult::~TestResult()
{}

TestResult &TestResult::operator=(const TestResult &rhs)
{
    if (this != &rhs) {
        _caseName = rhs._caseName;
        _type = rhs._type;
        _signal = rhs._signal;
    }
    return *this;
}

TestResult::TestResult(const std::string &caseName)
    : _caseName(caseName)
    , _type(NONE)
    , _signal(0)
{}

void TestResult::setType(e_result_type type)
{
    if (type >= MAX_TYPENUM || type <= MIN_TYPENUM)
        type = NONE;
    _type = type;
}

void TestResult::setSignal(int signal)
{

    _signal = signal;
}

std::string TestResult::getText(void) const
{
    static std::map<int, std::string> signalMap;
    static std::map<e_result_type, std::string> typeMap;
    static bool initialized = false;

    if (initialized == false)
    {
        initialized = true;

        signalMap[SIGINT] = "SIGINT";
        signalMap[SIGILL] = "SIGILL";
        signalMap[SIGFPE] = "SIGFPE";
        signalMap[SIGSEGV] = "SIGSEGV";
        signalMap[SIGTERM] = "SIGTERM";
        signalMap[SIGABRT] = "SIGABRT";
        signalMap[SIGALRM] = "TLE";

        typeMap[NONE] = "NONE";
        typeMap[OK] = "OK";
        typeMap[NG] = "NG";
        typeMap[SIGNAL] = "SIGNAL";
        typeMap[INTERNAL] = "INTERNAL";
    }

    if (_type != SIGNAL)
        return typeMap[_type];
    try {
        return signalMap.at(_signal);
    }
    catch (const std::exception &e)
    {}
    return typeMap[_type];
}

void TestResult::print(std::ostream &o) const
{
    std::string color;
    std::string msg;

    msg = getText();
    if (msg == "OK")
        color = "\e[32m"; // GREEN
    else if (msg == "TLE" || msg == "INTERNAL")
        color = "\e[33m"; // YELLOW
    else
        color = "\e[31m"; // RED

    o << _caseName << ":";
    if (isColored)
        o << color;
    o << "[" << msg << "]";
    if (isColored)
        o << "\e[m"; // Clear Color
    o << std::endl;
}

std::ostream &operator<<(std::ostream &o, TestResult &rhs)
{
    rhs.print(o);
    return o;
}
