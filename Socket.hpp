#ifndef  SOCKET_HPP
#define  SOCKET_HPP

#include "utils/Tools.hpp"
#include "Server.hpp"


class Socket 
{    
    int _port;
    struct sockaddr_in hint;
    int  fd;
public: 
    Socket(int const &port);
    ~Socket();

    int CreateServerSocket(); //create  a server socket 
    int BindSock(int const &socket); //bind   socket 
    static int AccectSock(int const &socket); //  accept socket 

    void SetSockAddress();
    int const &getFd()const ;

};

#endif
