#include "Socket.hpp"

Socket::Socket(Server serv)
{
    server = serv;
    SetSockAddress();
    fd = CreateServerSocket();

    fd = BindAccectSock(fd);
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

int Socket::BindAccectSock(int socket)
{
    std::cout << "bind accept  .." << std::endl;

    int new_socket=0;
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    if (bind(socket,(struct sockaddr *)&hint,sizeof(hint)) < 0) 
    { 
        perror("“bind failed”"); 
        return 0; 
    }

    if (listen(socket, 10) < 0) 
    { 
        perror("“In listen”"); 
        exit(0);
    }
    std::cout << "salam";
    if ((new_socket = accept(socket, (struct sockaddr *)&client, (socklen_t*)&clientSize))<0)
    {
        perror("In accept");            
        exit(0);        
    }
    close (socket);
    return (new_socket);
}

    int Socket::getFd()const 
    {
        return fd;
    }