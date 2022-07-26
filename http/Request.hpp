#pragma once

#include "../utils/Tools.hpp"

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
        std::string const & getBody() const{
            return body;
        }
       size_t  getContentLength() const{
            return content_length;
        }
        std::string const & getContentType() const{
            return content_type;
        }
        std::map<std::string, std::string> getHeaders() const 
        {
            return headers;
        }

        std::string getCookie() 
        {
            std::map<std::string, std::string>::iterator it =  headers.find("Cookie");
            if (it != headers.end())
                return (it->second);
            return "";
        }

        // void validUri(std::string s);

        // Request(Request  const & src);
        Request &operator=(Request  const & src);

        void    debug();
};