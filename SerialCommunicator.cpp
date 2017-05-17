#include <errno.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdlib>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "SerialCommunicator.hpp"

using namespace std;

SerialCommunicator::SerialCommunicator()
{
    open();
}

void SerialCommunicator::open()
{
    if ((fd = serialOpen ("/dev/serial0", 9600)) < 0)
    {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
        exit(1);
    }

    if (wiringPiSetup () == -1)
    {
        fprintf (stdout, "Unable to start wiringPi: %s\r\n", strerror (errno)) ;
        exit(1);
    }

    serialFlush(SerialCommunicator::fd);
    cout << "UART communication has been initiated.\r\n";
}

bool SerialCommunicator::receive()
{
    do
    {
        SerialCommunicator::data = serialGetchar(SerialCommunicator::fd);

        cout << "Data received: " << SerialCommunicator::data << "\r\n";

        if(SerialCommunicator::data == -1)
        {
            cout << "Restarting serial communication\r\n";
            serialClose(SerialCommunicator::fd);
            open();
        }

    } while (SerialCommunicator::data != 'Y');

    return true;
}

char SerialCommunicator::getData()
{
    return SerialCommunicator::data;
}

void SerialCommunicator::send(char data)
{
    serialFlush(SerialCommunicator::fd);
    serialClose(SerialCommunicator::fd);
    open();
    cout << "Sending Response " << data << "\r\n";
    serialPutchar(SerialCommunicator::fd, 'X');
    serialPutchar(SerialCommunicator::fd, data);
}
