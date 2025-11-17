#ifndef CALL_HPP
#define CALL_HPP

#include "enums.hpp"
#include <cstdint>
#include <string>

class Call
{
	public:
        Call(){};
        Call(CallType type): type_(type) {}
        std::string* number(){return &number_;}
		CallStatus status(){return status_;}
		CallMode mode(){return mode_;}
		uint8_t numberType(){return numberType_;}
        CallType type(){return type_;}
		void number(std::string n){number_=n;}
        void status(int i){status_=static_cast<CallStatus>(i);}
        void mode(int i){mode_=static_cast<CallMode>(i);}
		void numberType(uint8_t i){numberType_=i;}
        //for test purposes
        ~Call(){};
	private:
		std::string number_;
		CallStatus status_=CallStatus::UNKNOWN;
		CallMode mode_=CallMode::UNKNOWN;
		uint8_t numberType_;
        CallType type_=CallType::UNKNOWN;
};

#endif
