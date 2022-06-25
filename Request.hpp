#pragma once

#include <iostream>
#include <map>
#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <string>
#include <unistd.h>
class Request
{
    private:
        std::map<std::string, std::string> headers;
        std::string Muv;
        int  _bodyfd;
        int ischuncked;
      
    public:
        bool isFrstRead;
        bool isslastRead;
        Request();
        ~Request();
        void    fillRequest(char *buff,  int read);
        void fillHeaders(std::string header);
        void fillBody( char *buff, int read, int _bodyfd);

        // Request(Request  const & src);
        // Request opertor=(Request  const & src);
        void    debug();
};