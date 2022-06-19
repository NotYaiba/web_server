#include "Socket.hpp"

Socket::Socket(Server serv)
{
    server = serv;
}
Socket::~Socket( )
{

}
int Socket::CreateServerSocket()
{
    std::cout << "Creating server socket..." << std::endl;
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        throw  "Can't create a socket!";
    }
    return listening;
}
int Socket::SetSockAddress()
{
    std::cout << "SetSockAddress .." << std::endl;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(server.getPort());
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
}

