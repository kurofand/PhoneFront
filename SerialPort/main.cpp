#include <iostream>
//#include "SerialPort.cpp"
#include "Phone.cpp"
#include <thread>
#include <unistd.h>

void listen(Phone &phone)
{
	const size_t bufSize=255;
	for(;;)
	{
		char buf[bufSize];
		int read=phone.port()->readFromPort(buf, bufSize);
		if(read>0)
		{
			std::string resp(buf, read);
			phone.parseResponse(resp);
//			std::cout<<resp<<std::endl;
			memset(buf, 0, bufSize);
		}
		else if(read<0)
		{
			int e=errno;
			std::cout<<"An error occured on read: "<<strerror(e)<<std::endl;
			break;
		}
	}
}

int main()
{
	SerialPort serialPort{"/dev/ttyUSB2", 9600};
	if(serialPort.open())
	{
		Phone phone(&serialPort);
		std::thread tListen(listen, std::ref(phone));
		tListen.detach();
		phone.requestNumber();
		sleep(1);
		phone.requestSignalStrength();
		sleep(1);
		phone.requestConnectionStatus();
		sleep(20);
/*		if(port.writeToPort("AT+CREG?\r\n")<0)
//		if(port.writeToPort("AT+CGPS?\r\n")<0)
		{
			std::cout<<"error on write"<<std::endl;
			port.close();
			return -1;
		}

		size_t bufSize=255;

		std::cout<<"Start reading..."<<std::endl;
		for(;;)
		{
			char buf[bufSize];
			int read=port.readFromPort(buf, bufSize);
//			std::cout<<read<<std::endl;
			if(read>0)
			{
				std::cout<<"Read chunk of data, size: "<<read<<", data:"<<std::endl;
				std::cout<<std::string(buf, read)<<std::endl;
				memset(buf, 0, bufSize);
			}
			else if(read<0)
			{
				int e=errno;
				std::cout<<"An error occured on read: "<<strerror(e)<<std::endl;
				break;
			}
		}*/

/*		char buf[1024];
		int readed=port.readFromPort(buf, 1024);
		if(readed>-1)
		{
			std::cout<<std::string(buf, readed)<<std::endl;
			memset(buf, 0, 1024);
			std::cout<<"Wait for signal from modem..."<<std::endl;
			readed=-1;
			for(;;)
			{
				readed=port.readFromPort(buf, 1024);
				if(readed>0)
				{
					std::cout<<std::string(buf, readed)<<std::endl;
					memset(buf, 0, 1024);
				}
			}
		}
		else
		{
			int e=errno;
			std::cout<<strerror(e)<<std::endl;
		}*/

/*		if(port.writeToPort("AT+CGPSINFO\r\n")<0)
		{
			std::cout<<"error"<<std::endl;
			port.close();
			return -1;
		}
		memset(buf, 0, 100);
		readed=port.readFromPort(buf, 100);
//		std::cout<<readed<<std::endl;
		if(readed<0)
		{
			port.close();
			return -1;
		}
		std::string str{std::string(buf, readed)};
		std::cout<<str<<"|"<<std::endl;*/
		serialPort.close();
		std::cout<<"Port closed, exit..."<<std::endl;
	}
	return 0;
}
