#pragma once

#include <iostream>
#include <map>
#include <fstream>
class Request
{
    private:
        std::map<std::string, std::string> headers;
        std::string Muv;
        bool isFrstRead;
      
    public:
        Request();
        ~Request();
        void    fillRequest(char *buff,  int read);
        void fillHeaders(std::vector <std::string> vect);
        
        // Request(Request  const & src);
        // Request opertor=(Request  const & src);
        void    debug();
};