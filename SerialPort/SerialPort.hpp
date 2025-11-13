#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

class SerialPort
{
	public:
		SerialPort(const char* port, unsigned speed);
		bool open();
		int writeToPort(const char* msg);
		int readFromPort(char* buf, size_t s);
		void close();
		~SerialPort(){close();}
	private:
		const char* port;
		unsigned speed;
		int portDescriptor=-1;
};

#endif
