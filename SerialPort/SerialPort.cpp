#include "SerialPort.hpp"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

SerialPort::SerialPort(const char* port, unsigned speed)
{
	this->port=std::move(port);
	this->speed=speed;
}

//TODO: add error handling
bool SerialPort::open()
{
//NONBLOCK cases "Resource temporarily unavailable" or read()
//	if((portDescriptor=::open(port, O_RDWR|O_NOCTTY|O_NONBLOCK))<0)
	if((portDescriptor=::open(port, O_RDWR|O_NOCTTY|O_SYNC))<0)
		return false;

	struct termios tty;
	if(tcgetattr(portDescriptor, &tty)!=0)
		return false;

	cfsetospeed(&tty, speed);
	cfsetispeed(&tty, speed);

//TODO: check flags in examples, priority to example ones
	tty.c_cflag=(tty.c_cflag & ~CSIZE)|CS8; // 8-bit characters
	tty.c_iflag&=~IGNBRK; // disable break processing
	tty.c_lflag=0; // no signaling chars, no echo, no
                     // canonical processing
	tty.c_oflag=0; // no remapping, no delays
	tty.c_cc[VMIN]=0; // read doesn't block
	tty.c_cc[VTIME]=5; // 0.5 seconds read timeout

	tty.c_iflag&=~(IXON|IXOFF|IXANY); // shut off xon/xoff ctrl

	tty.c_cflag|=(CLOCAL|CREAD); // ignore modem controls,
                             // enable reading
	tty.c_cflag&=~(PARENB|PARODD); // shut off parity
	tty.c_cflag&=~CSTOPB;
	tty.c_cflag&=~CRTSCTS;

//TODO: add error handling
	if(tcsetattr(portDescriptor, TCSANOW, &tty)!=0)
		return false;

//	readFromPort(nullptr, 100);

	return true;
}

int SerialPort::writeToPort(const char* msg)
{
	return write(portDescriptor, msg, strlen(msg));
}

int SerialPort::readFromPort(char* buf, size_t s)
{
	return read(portDescriptor, buf, s);
}

void SerialPort::close()
{
	::close(portDescriptor);
}
