#ifndef  WEBSERVER_HPP
#define  WEBSERVER_HPP

#include <fstream>
#include <vector>
#include <map>
#include "Server.hpp"
#include "Connection.hpp"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

// class Connextion;
class Webserver 
{   
    Connection _cnx;
    fd_set readset;
    fd_set writeset;
    fd_set readcopy;
    fd_set writecopy;
    int maxfd;
    std::map < int, int > fd_map;
public: 
    Webserver(Connection const &connection);
    ~Webserver();

    void InitData(Connection const &connection); //create  a server socket 
    void RunWebServer();
    void NewConnectionRead(int fd);
    void HandleResponse(int fd);
    void HandleRequest(int fd);

};

#endif
