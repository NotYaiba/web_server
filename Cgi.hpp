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
    std::string _header;
    std::string _status;
    std::string _location;
    char **arr;
    bool on;
    std::map<std::string, std::string> cgimap ;
    char** env;
    std::string filetype;
    std::string filepath;
    std::string toRender_file;
    bool is_post;
    public :
    Cgi(Server serv , Request  req ,  Location const & loc);
    void initData();
    void execute_cgi();
    void SetEnv();
    char **initarr();
    bool getOn()const 
    {
        return on;
    }
    std::string find(std::string, std::string);
    void pars_file();
    int getStatus() const;
    std::string getHeader() const;
    std::string getLocation() const;
    std::string gettoRender_file() const;
    void dupp_file(std::string filename);

};

