#ifndef CLI_HPP
#define CLI_HPP

#include <string>

class Cli
{
    public:
        Cli(const std::string *num): number(num){};
        bool getNumInfo();

    private:
        const std::string *number;

};

#endif // CLI_HPP
