#ifndef SMS_HPP
#define SMS_HPP

#include <string>

class Sms
{
    public:
        Sms(std::string *pdu): pdu_(pdu){};
        void parse();
        void saveToDB();
        const std::string* message(){return &message_;}
        std::string* number(){return &number_;}
        const std::string* datetime(){return &datetime_;}
        std::string* toPdu();
        std::string* pdu(){return pdu_;}

    private:
        std::string *pdu_=nullptr;
        std::string message_;
        std::string number_;
        std::string datetime_;
        bool received_;

        void strHexToDec(size_t *p, size_t pos);
};

#endif // SMS_HPP
