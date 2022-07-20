#pragma once


#include "../utils/Tools.hpp"

#include "../Server.hpp"
#include "Request.hpp"
#include "../Location.hpp"

class Response
{
    private:
    bool isEndRes;
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
    std::string file_name;
    size_t file_size;
    std::string file_type;
    std::map<std::string, std::string> cgimap ;

    int flag;
    int written;

    std::pair<int , std::string> statusCode;
    std::pair<char* , size_t> bufpair;
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
    std::pair<char * , size_t> getHeader();
    std::string generateBody();
    void setStatusCode(int code);
    void generateredeHeader();
    void getIndex(std::string path);
    bool isDir(std::string path);
    void initData(Server  serv , Request req);
    void setIsvalid();
   std::pair<char* , size_t> getBody();
   bool getIsend() const
   {
       return (isEndRes);
   }

    Response &operator=(Response  const & src)
    {
        isEndRes = src.getIsend();
        return *this;
    }

    
};