#ifndef  SERVER_HPP
#define  SERVER_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "Location.hpp"

class Location;
class Server 
{    
    std::string serverName;
    std::string port;
    std::string host;
    std::string body_size_limit;
    std::vector<Location> locations;
public: 
    Server();
    ~Server();

    void setServerName(std::string const &servername);
    std::string const & getServerName() const;

    void setPort(std::string const &portt);
    std::string const & getPort() const;

    void setHost(std::string const &hostt);
    std::string const & getHost() const;

    void setBody_size_limit(std::string const &body_size_limitt);
    std::string const & getBody_size_limit() const;
    
    void locationADD(Location  loc);
    std::vector<Location> const &  getLocations() const ;

};

#endif
