#include "Server.hpp"


    Server::Server()
    {

    }
    Server::~Server()
    {

    }
    void Server::addServerName(std::string const &servername) 
    {
        serverNames.push_back(servername);
    }
    std::string const & Server::getServerName() const
    {
        return (serverNames);
    }

    void Server::setPort(std::string const &rot) 
    {
        port = rot;
    }
    std::string const & Server::getPort()const 
    {
        return port;
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
    void Server::setErrorpage(std::string const &error)
    {
        error_page = error;

    }
    std::string const & Server::getErrorpage() const
    {
        return error_page;
    }

    void Server::locationADD(Location   loc)
    {
        locations.push_back(loc);
    }

    std::vector<Location> const &  Server::getLocations()const 
    {
        return locations;
    }
    void Server::addtoCgiMap(std::string key, std::string val)
    {
        cgimap.insert(std::make_pair(key, val));
    }

    std::map<std::string, std::string> const &  Server::getCgiMap() const 
    {
        return cgimap;
    }
