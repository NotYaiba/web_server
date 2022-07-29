#include "Socket.hpp"

Socket::Socket(int const &port)
{
    _port = port;
    SetSockAddress();
    fd = CreateServerSocket();
    fd = BindSock(fd);
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
void Socket::SetSockAddress()
{
    hint.sin_family = AF_INET;
    hint.sin_port = htons(_port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
}

int Socket::BindSock(int const &socket)
{

    int flag = 1;  
    if (-1 == setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {  
        perror("setsockopt fail");  
    }  
        fcntl(socket, F_SETFL,  O_NONBLOCK);
    if (bind(socket,(struct sockaddr *)&hint,sizeof(hint)) < 0) 
    { 
        perror("“bind failed”"); 
        exit (0); 
    }

    if (listen(socket, 100) < 0) 
    { 
        perror("“In listen”"); 
        exit(0);
    }
    return (socket);
}

int Socket::AccectSock(int const &socket)
{
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int new_socket = accept(socket, (struct sockaddr *)&client, &clientSize);
    if (new_socket == -1)
    {
        std::cerr << "Problem with client connecting!";
        return -4;
    }
    return (new_socket);
}

int const &Socket::getFd()const 
{
    return fd;
}