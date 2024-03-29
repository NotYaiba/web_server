#ifndef  SERVER_HPP
#define  SERVER_HPP

#include "./utils/Tools.hpp"

#include "Parser.hpp"
#include "Location.hpp"

class Location;
class Server 
{    
    std::vector< std::string> serverNames;
    int port;
    int line;
    std::string host;
    int body_size_limit;
    std::string error_page;
    std::map<std::string, std::string> cgimap;
    std::vector<Location> locations;
public: 
    Server();
    ~Server();

    void addServerName(std::string const &servername);
     std::vector< std::string> const & getServerName() const;

    void setPort(std::string const &portt);
    int const & getPort() const;

    void setHost(std::string const &hostt);
    std::string const & getHost() const;

    void setBody_size_limit(std::string const &body_size_limitt);
    int const & getBody_size_limit() const;
    void setErrorpage(std::string const &error);
    std::string const & getErrorpage() const;
    
    void locationADD(Location  loc);
    void setLine(int const & linne)
    {
        line = linne;
    }
    std::vector<Location> const &  getLocations() const ;

    void   debug();
    void   clear()
    {
        locations.clear();
        serverNames.clear();
        cgimap.clear();
        port = 0;
        host = "";
        body_size_limit = 10;
        error_page = "";
    }
    void throwError(int type, std::string para);

};

#endif
