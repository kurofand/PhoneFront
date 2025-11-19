#include "sms.hpp"
#include <iostream>
#include <sstream>

void Sms::parse()
{
    std::cout<<"parsing"<<std::endl;
    size_t numLength=0;
    strHexToDec(&numLength, 0);

    //skip sms center number and two next digit(First octet of this SMS-DELIVER message.)
    size_t currentPos=2+numLength*2+2;

    strHexToDec(&numLength, currentPos);
    //in pdu format phone numbers are always even - if it is not there will be "F" in the tail
    //but in this case number's length will be odd, so if length is odd inc it to read full number
    //Example: number is 11223344556, in pdu format it will become 1122334455F6 with 0B(11) length
    //if read 11 digits last digit will be lost, so inc length to even(12), read 12 digits(include F)
    //and after decoding cut F
    if(numLength%2!=0)
        numLength++;
    std::string num=pdu_->substr(currentPos+=2, numLength+2);
    //91 means the number is in international format which means "+" on the head
    //another value means that number is actually a text, but it is probably not a SMS case
    if(num.substr(0, 2)=="91")
        number_+='+';
    for(unsigned i=2;i<=numLength;i+=2)
    {
        number_.push_back(num.at(i+1));
        number_.push_back(num.at(i));
    }


}

void Sms::strHexToDec(size_t *p, size_t pos)
{
    std::stringstream ss;
    ss<<std::hex<<pdu_->substr(pos, 2);
    ss>>*p;
}
