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
     std::vector< std::string> const &Server::getServerName() const
    {
        return (serverNames);
    }

    void Server::setPort(std::string const &rot) 
    {
        port = stoi(rot);
    }
    int const & Server::getPort()const 
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
void    Server::debug()
{
    std::cout  <<blue << "serverNames :" << reset  << serverNames[0] << std::endl; 
    std::cout  <<blue << "port :" << reset  << port << std::endl; 
    std::cout  <<blue << "host :" << reset  << host<< std::endl; 
    std::cout  <<blue << "error_page :" << reset  << error_page << std::endl; 
    // std::cou<<t << blue << "cgimap :" << reset << setw(50) << defaultt <<  std::endl; 
    for (size_t i = 0 ; i < locations.size() ; i++)
        locations[i].debug();
}