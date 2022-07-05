#pragma once

#include <iostream>
#include <map>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include "Server.hpp"
# include <sys/types.h>
# include <sys/stat.h>
#include <dirent.h>
#include "Request.hpp"
#include "Location.hpp"
#define _POSIX_SOURCE
class Response
{
    private:
    Server _server;
    Request _req;
    std::string  _path;
    std::string _loc;
    int validMethod;
    std::string _method;
    std::string header;
    std::string body;
    std::string _redirect;
    std::string _def;
    int flag;

    std::pair<int , std::string> statusCode;
    Location  matching_location;
    public:
    Response(Server  serv , Request req);
    std::vector<Location> locations;
    void    debug();
    void findMatchingLocation();
    void getLocation();
    void getMethod();
    void KaynatMethod();
    void Delete();
    void Get();
    void Post();
    void generateHeader();
    std::string gethadak();
    std::string generateBody();
    void setStatusCode(int code);
    void generateredeHeader();
void getIndex(std::string path);
bool isDir(std::string path);


    
};