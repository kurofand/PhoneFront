#include "sms.hpp"
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <regex>

#include <chrono>
#include <iomanip>

#include "../sqliteconnector/sqliteclient.hpp"
#include "../Log/log.hpp"

void Sms::parse()
{
    Log(LogLevel::Debug)<<"Start pdu parse";
    size_t numLength=0;
    strHexToDec(&numLength, 0);

    //skip sms center number
    size_t currentPos=2+numLength*2;

    //There can be header in UD, it can be determined by PDU type octets
    bool udhPresent=false;
    {
        int pduType;
        strHexToDec(&pduType, currentPos);
        udhPresent=pduType&(1<<6);
    }
    currentPos+=2;

    strHexToDec(&numLength, currentPos);
    //in pdu format phone numbers are always even - if it is not there will be "F" in the tail
    //but in this case number's length will be odd, so if length is odd inc it to read full number
    //Example: number is 11223344556, in pdu format it will become 1122334455F6 with 0B(11) length
    //if read 11 digits last digit will be lost, so inc length to even(12), read 12 digits(include F)
    //and after decoding cut F
    if(numLength%2!=0)
        numLength++;

    //91 means the number is in international format which means "+" on the head
    //another value means that number is actually a text, but it is probably not a SMS case
    if(pdu_->substr(currentPos+=2, 2)=="91")
        number_+='+';
    {
        auto numStart=currentPos;
        for(currentPos+=2;currentPos<=numStart+numLength;currentPos+=2)
        {
            number_.push_back(pdu_->at(currentPos+1));
            number_.push_back(pdu_->at(currentPos));
        }
        if(number_.ends_with('F'))
            number_.pop_back();
    }
    Log(LogLevel::Debug)<<"Number: "<<number_;

    //skip TP-PID - protocol identifier
    currentPos+=2;
    //TP-DCS - data coding scheme, 2 digits - "08" for UCS2 and "00" for 7bit encoding
    bool isUcs=pdu_->substr(currentPos, 2)=="08";
    currentPos+=2;

    //get datetime, same as sender number octets are reversed, format is yy mm dd HH MM SS
    //timezone info - last 2 digits, skip
    {
        auto dateStart=currentPos;
        std::string tmp;
        for(;currentPos<=dateStart+12;currentPos+=2)
        {
            tmp.push_back(pdu_->at(currentPos+1));
            tmp.push_back(pdu_->at(currentPos));
        }
        std::string year, mon, day, hour, min, sec;
        year="20"+tmp.substr(0, 2);
        mon=tmp.substr(2, 2);
        day=tmp.substr(4, 2);
        hour=tmp.substr(6, 2);
        min=tmp.substr(8, 2);
        sec=tmp.substr(10, 2);
        datetime_=day+"/"+mon+"/"+year+" "+hour+":"+min+":"+sec;

    }
    Log(LogLevel::Debug)<<"Datetime: "<<datetime_;

    //skip TP-UDL - length of data. for basic sms data will be from current point to the end of the message
    currentPos+=2;

    //skip header if present
    if(udhPresent)
    {
        int headerLength;
        strHexToDec(&headerLength, currentPos);
        currentPos+=2+headerLength*2;
    }

    if(isUcs)
        //data in 4 digit hexes
        for(;currentPos<pdu_->size();currentPos+=4)
        {
            int val;
            strHexToDec(&val, currentPos, 4);
            std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
            message_+=converter.to_bytes(val);
        }
    else
    {
        //data in 2 digit hex septets
        uint8_t iteration=1;
        std::string prevVal, currVal;
        int val;
        strHexToDec(&val, currentPos);
        prevVal=std::bitset<8>(val).to_string();
        for(currentPos+=2;currentPos<=pdu_->size();currentPos+=2)
        {
            strHexToDec(&val, currentPos);
            currVal=std::bitset<8>(val).to_string();
            currVal.append(prevVal.substr(0, iteration));
            prevVal.erase(0, iteration);
            message_+=static_cast<char>(std::stoi(prevVal, nullptr, 2));
            prevVal=currVal;
            if(++iteration==7)
            {
                iteration=0;
                std::string tail=prevVal.substr(7);
                message_+=static_cast<char>(std::stoi(tail, nullptr, 2));
                prevVal.resize(7);
            }
        }
    }
    Log(LogLevel::Debug)<<"Message: "<<message_;
    received_=true;
    Log(LogLevel::Debug)<<"Pdu parsing complete";

}

std::string* Sms::toPdu()
{
    Log(LogLevel::Debug)<<"Creating pdu from sms object";
    //to awoid leak, but data will be lost
    if(pdu_!=nullptr)
        delete pdu_;
    pdu_=new std::string();
    //add default SCA
    *pdu_+="00";

    //PDU-type section, using default params:
    //RP - not defined
    //UDHI - SMS body only
    //SRR - not requested
    //VPF - no VP field
    //RD - resend
    //MTI - sended SMS
    //00000001 -> 0x01
    *pdu_+="01";

    //MR - default 00
    *pdu_+="00";

    //DA section
    {
        auto r=std::regex("[^\\d]");
        std::string clearNumber=regex_replace(number_, r, "");
    //PL - number length, 2 digit hex. counts only numbers, ignoring all symbols including "+"
        {
            std::stringstream ss;
            ss<<std::hex<<clearNumber.length();
            auto tmp=ss.str();
            *pdu_+=tmp.length()>1?tmp:"0"+tmp;
        }
    //PT - 2 digit hex, 91 for international number format and 81 for local
        *pdu_+=number_[0]=='+'?"91":"81";
    //RP - number, same logic with decoding pdu on receive message
        if(clearNumber.length()%2!=0)
            clearNumber+='F';
        for(unsigned i=0;i<clearNumber.length();i+=2)
        {
            pdu_->push_back(clearNumber[i+1]);
            pdu_->push_back(clearNumber[i]);
        }
    }

    //PID section, default
    *pdu_+="00";

    //DCS section, set UCS2, default messages
    *pdu_+="08";

    //VP not used, so just skip
    //UD section
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring msg=converter.from_bytes(message_);
        //UDL section, 2 byte per symbol in hex
        //wstring for get actual size of non latin chars string
        {
            std::stringstream ss;
            ss<<std::hex<<msg.length()*2;
            *pdu_+=ss.str();
        }
        for(auto &c: msg)
        {
            std::stringstream ss;
            ss<<std::hex<<static_cast<int>(c);
            std::string hex=ss.str();
            while(hex.size()<4)
                hex="0"+hex;
            *pdu_+=hex;
        }
    }

    //not part of pdu, but message sent time should be in db
    //not chrono+format bc my rpy has outdated gcc w/o std::format
    auto now=std::chrono::system_clock::now();
    auto timet=std::chrono::system_clock::to_time_t(now);
    auto tm=*std::localtime(&timet);
    std::stringstream ss;
    ss<<std::put_time(&tm, "%d/%m/%Y %H:%M:%S");
    datetime_=ss.str();
    //also mark instance as sent message
    received_=false;
    Log(LogLevel::Debug)<<"Pdu completed";

    return pdu_;
}

void Sms::saveToDB()
{
    auto *dbInstance=SqliteClient::instance();
    dbInstance->connect();
    std::string query="INSERT INTO sms(isReceived, datetime, number, msg, isRead) "
                        "VALUES(";
    query+=received_+'0';
    query+=",\""+datetime_+"\",\""+number_+"\",\""+message_+"\",0);";
    dbInstance->executeQuery(query.c_str());

}

void Sms::strHexToDec(auto *p, size_t pos, size_t size)
{
    std::stringstream ss;
    ss<<std::hex<<pdu_->substr(pos, size);
    ss>>*p;
}
