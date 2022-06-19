#ifndef  SOCKET_HPP
#define  SOCKET_HPP

#include <fstream>
#include <vector>
#include <map>
#include "Server.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

class Socket 
{    
    Server server;
    struct sockaddr_in hint;
    int  fd;
public: 
    Socket(Server serv);
    ~Socket();

    int CreateServerSocket(); //create  a server socket 
    int BindAccectSock(int socket); //bind and accept socket 

    void SetSockAddress();
    int getFd()const ;

};

#endif
