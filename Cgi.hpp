#pragma once
#include "Server.hpp"
#include "http/Request.hpp"
#include "Location.hpp"
#include "utils/Tools.hpp"
class Cgi {
    Server _server;
    Request _req;
    Location _loc;
    std::map<std::string, std::string> cgimap ;
    public :
    Cgi(Server serv , Request  req ,  Location loc);
    void initData(Server serv , Request  req ,  Location loc);

};

