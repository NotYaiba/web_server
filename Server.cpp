#include "Server.hpp"


    Server::Server()
    {

    }
    Server::~Server()
    {

    }
    void Server::setServerName(std::string const &servername) 
    {
        serverName = servername;
    }
    std::string const & Server::getServerName() const
    {
        return (serverName);
    }
    void Server::setHost(std::string const &hostt)
    {
        host = hostt;
    }
    std::string const & Server::getHost() const
    {
        return host;
    }
    void Server::setBody_size_limit(std::string const &body_size_limitt)

    {
        body_size_limit = body_size_limitt;
    }
    std::string const & Server::getBody_size_limit() const
    {
        return body_size_limit;
    }

    void Server::locationADD(Location   loc)
    {
        locations.push_back(loc);
    }

    std::vector<Location> const &  Server::getLocations()const 
    {
        return locations;
    }
