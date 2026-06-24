#ifndef CLI_HPP
#define CLI_HPP

#include <string>

#include <libxml/HTMLparser.h>

class Cli
{
    public:
        Cli(const std::string *num): number(num){};
        bool getNumInfo();
        std::string* name(){return &name_;}

    private:
        xmlNode* findNode(xmlNode *node, const char* tag, const char* propName=nullptr, const char* propVal=nullptr);
        const std::string *number;
        std::string name_;

};

#endif // CLI_HPP
