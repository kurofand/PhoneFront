#ifndef PHONE_HPP
#define PHONE_HPP

#include "enums.hpp"
#include "call.hpp"
#include "SerialPort.cpp"
#include "../sqliteconnector/sqliteclient.cpp"
#include <qqmlapplicationengine.h>

class Phone
{
	public:
        //Phone(SerialPort *p, QQmlApplicationEngine *e): port_(p),  engine_(e) {};
        Phone(){};
        Phone(SerialPort *p): port_(p) {};
        static Phone* getInstance();
		void requestNumber();
		void requestOperatorInfo();
		void requestConnectionStatus();
		void requestSignalStrength();
        void setVoiceHangupControl();
        void readAndDeleteMessage(const char* mesId);
		void parseResponse(std::string &str);
		void answer();
		void disconnect();
		void hangUp();
        void call(std::string &&num);
        void port(SerialPort *p){port_=p;}
        void engine(QQmlApplicationEngine *engine){engine_=engine;}
		SerialPort* port(){return port_;}
        ~Phone(){};
	private:
        QObject* findQMLObj(const char* objName);
        void sendNotification(std::string *body);
        inline static Phone *instance=nullptr;
        SerialPort *port_=nullptr;
		std::string number_;
		std::string operatorName_;
		ConnectionStatus status_=ConnectionStatus::UNKNOWN;
		ConnectionType connectionType_=ConnectionType::UNKNOWN;
        Call *currentCall_=nullptr;
		//0-31
		uint8_t signalStrength_=0;

        QQmlApplicationEngine *engine_;
        SqliteClient client{"db/db.db"};

};


#endif
