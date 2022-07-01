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
# include <sys/types.h>
class Request
{
    private:
        std::map<std::string, std::string> headers;
        std::string Muv;
        int ischuncked;
        int invalidMethod;
        std::string Uri;
        std::string body;
        std::string method;
        int i;
        size_t content_length;
        std::string content_type;
        std::vector<char> god_vect;
        std::vector<std::string> method_vect;
        int status_code;
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
        void  createFile();
        void  InitData();
        void checkHeaders();
        int const & getStatusCode() const{
            return status_code;
        }
        std::string const & getUri() const{
            return Uri;
        }
        std::string const & getMethod() const{
            return method;
        }

        // void validUri(std::string s);

        // Request(Request  const & src);
        Request &operator=(Request  const & src);

        void    debug();
};