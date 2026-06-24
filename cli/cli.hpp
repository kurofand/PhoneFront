#ifndef CLI_HPP
#define CLI_HPP

#include <string>

#include <libxml/HTMLparser.h>

class Cli
{
    public:
        Cli(const std::string *num): number(num){};
        bool getNumInfo();

    private:
        xmlNode* findNode(xmlNode *node, const char* tag, const char* propName=nullptr, const char* propVal=nullptr);
        const std::string *number;

};

#endif // CLI_HPP
