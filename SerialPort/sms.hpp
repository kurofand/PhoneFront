#ifndef SMS_HPP
#define SMS_HPP

#include <string>

class Sms
{
    public:
        Sms(std::string *pdu): pdu_(pdu){};
        void parse();
    private:
        std::string *pdu_;
        std::string message_;
        std::string number_;
        std::string datetime_;
};

#endif // SMS_HPP
