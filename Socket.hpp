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
public: 
    Socket(Server serv);
    ~Socket();


};

#endif
