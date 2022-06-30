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
class Request
{
    private:
        std::map<std::string, std::string> headers;
        std::string Muv;
        int ischuncked;
        int invalidMethod;
        std::string Uri;
        std::string body;
        int i;
        int content_length;
        std::vector<char> god_vect;
        std::vector<std::string> method_vect;
        int j;
    public:
        bool isFrstRead;
        int  _bodyfd;
        bool isslastRead;
        Request();
        ~Request();
        void    fillRequest(char *buff,  int read);
        void fillHeaders(std::string header);
        void fillBody( char   *buff, int read, int _bodyfd);
        bool checkEndRequest( char const *buff, int read);
        bool isChunksize(size_t begin , size_t end, char * str);
        void fill_vect(char *buff , size_t read);
        void writeVect(int fd);
        bool findchunkSize();
        void fillMethod();
        // void validUri(std::string s);

        // Request(Request  const & src);
        // Request opertor=(Request  const & src);
        void    debug();
};