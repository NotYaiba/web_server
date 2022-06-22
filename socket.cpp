#include "Socket.hpp"
#include <fcntl.h>
Socket::Socket(Server const &serv)
{
    server = serv;
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
    std::cout << "SetSockAddress .." << std::endl;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(server.getPort());
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
}

int Socket::BindSock(int const &socket)
{
    std::cout << "bind sock  .." << std::endl;

    int new_socket=0;

        fcntl(socket, F_SETFL,  O_NONBLOCK);
    if (bind(socket,(struct sockaddr *)&hint,sizeof(hint)) < 0) 
    { 
        perror("“bind failed”"); 
        exit (0); 
    }

    if (listen(socket, SOMAXCONN) < 0) 
    { 
        perror("“In listen”"); 
        exit(0);
    }
    return (socket);
}

int Socket::AccectSock(int const &socket)
{
    std::cout << "Accect sock  .." << std::endl;
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int new_socket = accept(socket, (struct sockaddr *)&client, &clientSize);
    if (new_socket == -1)
    {
        std::cerr << std::strerror( errno)  << std::endl;
        std::cerr << "Problem with client connecting!";
        return -4;
    }
    return (new_socket);
}

    int const &Socket::getFd()const 
    {
        return fd;
    }