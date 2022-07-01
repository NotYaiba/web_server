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
#include "Request.hpp"
#include "Location.hpp"

class Response
{
    private:
    Server _server;
    Request _req;
    Location _loc;
    int validMethod;
    int statusCode;
    std::string  matching_location;
    public:
    Response(Server  serv , Request req);
    std::vector<Location> locations;
    void    debug();
    void findMatchingLocation();
    void getLocation();
    void getMethod();

};