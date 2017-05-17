#ifndef SERIALCOMMUNICATOR_HPP
#define SERIALCOMMUNICATOR_HPP

class SerialCommunicator
{
    public:
        SerialCommunicator();
        void open();
        bool receive();
        void send(char data);
        char getData();
    private:
        int fd;
        char data;
};

#endif // SERIALCOMMUNICATOR_HPP
