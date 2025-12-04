#include "Phone.hpp"
#include "maps.hpp"

#include <sstream>
#include <stdio.h>

void Phone::requestNumber()
{
	port_->writeToPort("AT+CNUM\r\n");
}

void Phone::requestOperatorInfo()
{
	port_->writeToPort("AT+COPS?\r\n");
}

void Phone::requestConnectionStatus()
{
    auto *root=findQMLObj("root");
    if(root)
        root->setProperty("updatingConnectionStatus", true);
	port_->writeToPort("AT+CREG?\r\n");
}

void Phone::requestSignalStrength()
{
    auto *root=findQMLObj("root");
    if(root)
        root->setProperty("updatingSignalStrength", true);
	port_->writeToPort("AT+CSQ\r\n");
}

void Phone::call(std::string &&num)
{
    std::string command("ATD"+num+";\r\n");
    currentCall_=new Call();
    currentCall_->number(num);
	port_->writeToPort(command.c_str());
}

void Phone::answer()
{
	port_->writeToPort("ATA\r\n");
}

void Phone::disconnect()
{
	port_->writeToPort("ATH\r\n");
}

void Phone::hangUp()
{
	port_->writeToPort("AT+CHUP\r\n");
}

void Phone::setVoiceHangupControl()
{
    port_->writeToPort("AT+CVHU=0\r\n");
}

void Phone::readAndDeleteMessage(const char* mesId)
{
    std::string command("AT+CMGRD=");
    command+=mesId;
    command+="\r\n";
    port_->writeToPort(command.c_str());
}

void Phone::sendSms(Sms *sms)
{
    std::string command("AT+CMGS=");
    //size exclude SCA part and divided by 2 since 2 symbol per byte
    command+=(sms->pdu()->size()-2)/2;
    command+='\r';
    command+=*sms->pdu();
    command+=static_cast<char>(26);
    command+="\r\n";
    port_->writeToPort(command.c_str());
    delete sms->pdu();
}

//TODO: maybe add AT+CPAS command to get current call status

void Phone::parseResponse(std::string &str)
{
	std::cout<<"Parsing response..."<<std::endl;
	std::string commandKey;
	std::string responseStr;
//most common requests return value like "command: response"
//but there are also non requested commands like "RING"
	if(str.find(':')!=std::string::npos)
	{
//first 2 symbols are <\r><\n>
		commandKey=str.substr(2, str.find(':')-2);
		responseStr=str.substr(str.find(':')+2);
		responseStr=responseStr.substr(0, responseStr.find("<\r><\n>"));
	}
	else
    {
        commandKey=str;
        removeNewLine(&commandKey);
    }
	std::cout<<"|"<<commandKey<<"|"<<std::endl;
	std::cout<<responseStr<<"|"<<std::endl;
	ATResponse command=responseMap.find(commandKey)!=responseMap.end()?responseMap[commandKey]:ATResponse::UNKNOWN;
	switch(command)
	{
		case ATResponse::CSQ:
		{
			std::cout<<"Response to CSQ request...";
			auto commPos=responseStr.find(',');
//TODO: error handling for failing parse signal value
			if(commPos!=std::string::npos)
				try
				{
					size_t convertedSize=0;
					signalStrength_=std::stoi(responseStr.substr(0, commPos), &convertedSize);
				}
				catch(const std::exception &e)
				{
					std::cout<<"failed"<<std::endl;
					break;
				}
			std::cout<<"parsed successfully. Result:"<<signalStrength_<<std::endl;
            auto *tSignalStrength=findQMLObj("tSignalStrength");
            if(tSignalStrength)
                tSignalStrength->setProperty("text", signalStrength_);
            auto *root=findQMLObj("root");
            if(root)
                root->setProperty("updatingSignalStrength", false);
			break;
		}
		case ATResponse::CNUM:
		{
			std::cout<<"Response to CNUM request...";
			auto commPos=responseStr.find(',');
			responseStr=responseStr.substr(commPos+1);
			commPos=responseStr.find(',');
			number_=responseStr.substr(0, commPos);
			std::cout<<"parsed successfully. Result:"<<number_<<std::endl;
			break;
		}
		case ATResponse::COPS:
		{
			std::cout<<"Response to COPS request...";
			std::stringstream ss(responseStr);
			std::string line;
            std::string arr[4];
			uint8_t i=0;
			while(getline(ss, line, ','))
				arr[i++]=line;
            operatorName_=arr[2];
			std::cout<<"parsed successfully. Result:"<<operatorName_<<std::endl;
            auto *tOperator=findQMLObj("tOperator");
            if(tOperator)
                tOperator->setProperty("text", operatorName_.c_str());
			break;
		}
		case ATResponse::CREG:
		{
			std::cout<<"Response to CREG request...";
            //erase OK\r\n
            removeNewLine(&responseStr);
            responseStr.erase(responseStr.end()-2, responseStr.end());
//usual response something like " 0,0", space at the beginning cutted above
//so if size differs from 3 something went wrong
//TODO: handle cases when size differs from 3
			if(responseStr.size()==3)
			{
                status_=static_cast<ConnectionStatus>(responseStr[2]-'0');
                auto *tStatus=findQMLObj("tConnectionStatus");
                if(tStatus)
                    tStatus->setProperty("text", connectionStatus.at(status_));
                //connectionType_=static_cast<ConnectionType>(responseStr[2]);
                //std::cout<<"parsed successfully. Result:"<<static_cast<uint8_t>(status_)<<", "<<static_cast<uint8_t>(connectionType_)<<std::endl;
			}
            auto *root=findQMLObj("root");
            if(root)
                root->setProperty("updatingConnectionStatus", false);
			break;
		}
		case ATResponse::RING:
		{
			std::cout<<"RING request came"<<std::endl;
            if(currentCall_==nullptr)
                currentCall_=new Call(CallType::INCOMING);
			break;
		}
        //This command for list current calls, but it also executes automatically on active call(dialed and received)
        //not best decision but for now ignore command if there is call with number info
        //received pattern: create call on RING, get number from first CLCC, then ignore
        //dialed pattern: create call on ATD, set number on ATD, ignore all CLCC
		case ATResponse::CLCC:
		{
            if(currentCall_&&currentCall_->number()->empty())
            {
                std::string s;
                std::stringstream ss(responseStr);
                std::string arr[7];
                uint8_t i=0;
                while(getline(ss, s, ','))
                    arr[i++]=s;
                try
                {
                    //TODO: add checking and handling for wrong sizes
                    size_t convertedSize=0;
                    currentCall_->status(std::stoi(arr[2], &convertedSize));

                    currentCall_->mode(std::stoi(arr[3], &convertedSize));

                    currentCall_->numberType(std::stoi(arr[6], &convertedSize));
                }
                catch(std::exception &e)
                {

                }

                currentCall_->number(arr[5].substr(1, arr[5].size()-2));
                //if number in contacts show contactName(number) else just number
                std::string *number;
                if(contacts_->find(*currentCall_->number())!=contacts_->end())
                    *number=contacts_->at(*currentCall_->number())+"("+*currentCall_->number()+")";
                else
                    number=currentCall_->number();

                auto *dIncomingCall=findQMLObj("dIncomingCall");
                if(dIncomingCall)
                {
                    auto *tIncomingNumber=dIncomingCall->findChild<QObject*>("tIncomingNumber");
                    tIncomingNumber->setProperty("text", number->c_str());
                    QMetaObject::invokeMethod(dIncomingCall, "open");
                }

                std::string nBody="\"Incoming call from "+*number+"\"";
                sendNotification(&nBody);

            }

			break;
		}
		case ATResponse::CLIP:
		{
			break;
		}
        //TODO: add to end call cases feature to save call history
        case ATResponse::VOICE_CALL:
        {
            std::cout<<"voice call ";
            if(responseStr.find("END:")!=std::string::npos)
            {
                std::cout<<"END";
                delete currentCall_;
                currentCall_=nullptr;
                auto *dCall=findQMLObj("dCall");
                if(dCall)
                {
                    auto min=dCall->property("min").toString().toStdString();
                    auto sec=dCall->property("sec").toString().toStdString();
                    std::string length=min+":"+sec;
                    currentCall_->callLength(std::move(length));
                    QMetaObject::invokeMethod(dCall, "close");
                }
                if(currentCall_)
                {
                    delete currentCall_;
                    currentCall_=nullptr;
                }
            }
            else if(responseStr.find("BEGIN")!=std::string::npos)
            {
                std::cout<<"BEGIN";
                auto *dCall=findQMLObj("dCall");
                if(dCall)
                {
                    std::cout<<"call dialog found"<<std::endl;
                    dCall->setProperty("connected", true);
                    dCall->setProperty("runTimer", true);
                }
            }
            std::cout<<std::endl;
            break;
        }
        case ATResponse::CMTI:
        {
            std::cout<<"CMTI request came"<<std::endl;
            //TODO:
            //There is a CMGRD command(read and delete) so parsing for a actual id
            //is not necessary, but probably it is better to support non first message
            readAndDeleteMessage("0");
            break;
        }
        case ATResponse::CMGRD:
        {
            std::cout<<"CMGRD response came"<<std::endl;
            uint8_t i=0;
            std::stringstream ss(responseStr);
            std::string pduLine;
            //get second line(response body) from response
            while(getline(ss, pduLine, '\n'))
                if(i++==1)
                    break;
            Sms sms{&pduLine};
            sms.parse();
            std::string *number;
            if(contacts_->find(*sms.number())!=contacts_->end())
                *number=contacts_->at(*sms.number())+"("+*sms.number()+")";
            else
                number=sms.number();
            std::string nBody="\"New message from "+*number+"\"";
            sendNotification(&nBody);
            sms.saveToDB();
            break;
        }
        case ATResponse::MISSED_CALL:
        {
            std::string time, num;
            auto spacePos=responseStr.find(' ');
            time=responseStr.substr(0, spacePos);
            responseStr.erase(0, spacePos+1);
            removeNewLine(&responseStr);
            num=responseStr;
            if(currentCall_!=nullptr&&currentCall_->number()->data()==num)
            {
                auto dIncomingCall=findQMLObj("dIncomingCall");
                if(dIncomingCall)
                    QMetaObject::invokeMethod(dIncomingCall, "close");
                currentCall_->missed(true);
                delete currentCall_;
                currentCall_=nullptr;
            }
            break;
        }
	};

/*
Incoming and missed call:
RING

+CLCC: 4,1,4,0,0,"08020866256",128
// id, dir, stat, mode, mpty, number, type
RING

MISSED_CALL: 11:53AM 08020866256
*/

/*
minicom incoming call:
RING

+CLCC: 4,1,4,0,0,num,128

VOICE CALL: BEGIN

OK

+CLCC: 4,1,0,0,0,num,128

ERROR

+CLCC: 4,1,6,0,0,num,128

VOICE CALL: END: 000008
*/
/*
minicom ATD command output:
+CLCC: 3,0,2,0,0,num,129

VOICE CALL: BEGIN

+CLCC: 3,0,0,0,0,num,129

+CLCC: 3,0,6,0,0,num,129

VOICE CALL: END: 000003
*/

/*
incoming SMS
+CMTI: "SM",9

+CPMS: 10,10,10,10,10,10

OK

*/
}

QObject* Phone::findQMLObj(const char* objName)
{
    if(!engine_)
        return nullptr;
    auto *window=engine_->rootObjects().first();
    if(!window)
        return nullptr;
    return window->findChild<QObject*>(objName);
}

void Phone::sendNotification(std::string *body)
{
    std::string notification="notify-send PhoneFront ";
    notification+=*body;
    system(notification.c_str());
}

Phone* Phone::getInstance()
{
    if(instance==nullptr)
        instance=new Phone();
    return instance;
}

void Phone::removeNewLine(std::string *s)
{
    std::erase(*s, '\r');
    std::erase(*s, '\n');
}
