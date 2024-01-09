#include <iostream>
#include <algorithm>
#include <Poco/Thread.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/socketStreamSocket.h>
#include <Poco/Net/SocketsocketStream.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/TCPServerConnectionFactory.h>

#define MAX_THREADS_NUM 16
#define PORT 28888
#define SLEEP_INTERVAL 1000

using Poco::Net::TCPServer;
using Poco::Net::StreamSocket;
using Poco::Net::SocketsocketStream;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;

class InputReverseServerConnection : public TCPServerConnection{
public:
    InputReverseServerConnection(const StreamSocket& socket) : TCPServerConnection(socket) {}

    void run()
    {
        SocketsocketStream socketStream(socket());
        socketStream << "Welcome to POCO TCP server. Enter you string: (max 255 symbols):\n";
        socketStream.flush();

        std::string str;
        std::getline(socketStream, str);

        if (str.size() > 255)
        {
            socketStream << "Error: the line is not to exceed 255 symbols." << std::endl;
            return;
        }

        std::reverse(str.begin(), str.end());
        socketStream << str << "\n";
    }
};

class InputReverseServerConnectionFactory : public TCPServerConnectionFactory
{
public:
    InputReverseServerConnection* createConnection(const StreamSocket& socket) override
    {
        return new InputReverseServerConnection(socket);
    }
};

int main()
{
    Poco::Net::TCPServerParams* serverParams = new Poco::Net::TCPServerParams();
    serverParams->setMaxThreads(MAX_THREADS_NUM);

    TCPServer server(new InputReverseServerConnectionFactory(), PORT, serverParams);
    server.start();

    while (true)
    {
        Poco::Thread::sleep(SLEEP_INTERVAL);
    }

    // following code will never execute
    //server.stop();
    return 0;
}
