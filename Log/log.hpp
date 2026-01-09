#ifndef LOG_HPP
#define LOG_HPP
#include <sstream>

enum class LogLevel
{
    Error,
    Warning,
    Info,
    Debug
};

class Log
{
    public:
        Log(LogLevel lvl);
        template <class T>Log& operator<<(T t)
        {
            stream<<t;
            return *this;
        };

        Log(const Log&)=delete;
        Log operator=(const Log&)=delete;
        Log(Log&&)=delete;
        Log operator=(Log&&)=delete;

        ~Log();
    private:
        LogLevel logLevel_;
        std::ostringstream stream;
};

#endif // LOG_HPP
