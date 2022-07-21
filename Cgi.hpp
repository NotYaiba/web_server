#pragma once
#include "Server.hpp"
#include "http/Request.hpp"
#include "Location.hpp"
#include "utils/Tools.hpp"
class Cgi {
    Server _server;
    Request _req;
    Location _loc;
    std::string path;
    std::string uri;
    std::string method;
    std::string cgikey;
    std::string query;
    bool on;
    std::map<std::string, std::string> cgimap ;
    char** env;
    std::string filetype;
    std::string filepath;
    public :
    Cgi(Server serv , Request  req ,  Location const & loc);
    void initData();
    void SetEnv();
    char **initarr();
    bool getOn()const 
    {
        return on;
    }

};

