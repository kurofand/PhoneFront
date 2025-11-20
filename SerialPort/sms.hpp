#ifndef SMS_HPP
#define SMS_HPP

#include <string>

class Sms
{
    public:
        Sms(std::string *pdu): pdu_(pdu){};
        void parse();
        const std::string* message(){return &message_;}
        const std::string* number(){return &number_;}
        const std::string* datetime(){return &datetime_;}

    private:
        std::string *pdu_;
        std::string message_;
        std::string number_;
        std::string datetime_;

        void strHexToDec(size_t *p, size_t pos);
};

#endif // SMS_HPP
