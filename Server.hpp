#ifndef  SERVER_HPP
#define  SERVER_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include "Location.hpp"

class Location;
class Server 
{    
    std::vector< std::string> serverNames;
    std::string port;
    std::string host;
    std::string body_size_limit;
    std::string error_page;
    std::map<std::string, std::string> cgimap;
    std::vector<Location> locations;
public: 
    Server();
    ~Server();

    void addServerName(std::string const &servername);
     std::vector< std::string> const & getServerName() const;

    void setPort(std::string const &portt);
    std::string const & getPort() const;

    void setHost(std::string const &hostt);
    std::string const & getHost() const;

    void setBody_size_limit(std::string const &body_size_limitt);
    std::string const & getBody_size_limit() const;
    void setErrorpage(std::string const &error);
    std::string const & getErrorpage() const;
    
    void locationADD(Location  loc);
    std::vector<Location> const &  getLocations() const ;
    void addtoCgiMap(std::string key, std::string val);
    std::map<std::string, std::string> const &  getCgiMap() const ;
};

#endif
