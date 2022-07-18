#ifndef  WEBSERVER_HPP
#define  WEBSERVER_HPP

#include "utils/Tools.hpp"

#include "Server.hpp"
#include "Connection.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
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
    std::map<int , Request> req_map;
    std::map<int , Response> res_map;
    std::map<int , std::vector<Server> > servers;
    Request req;
    
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
