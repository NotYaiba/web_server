#include "Server.hpp"
#include "./utils/Tools.hpp"

    Server::Server() : port(0), host("") , body_size_limit(0) , error_page("")
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
        if (port == 0)
            port = stoi(rot);
        else
            throw "Duplicated parameter port";
    }
    int const & Server::getPort()const 
    {
        return port;
    }

    void Server::setHost(std::string const &hostt)
    {
            std::cout << lineS.getLine() << std::endl;
        if (host == "")
            host = hostt;
        else
        {
            throw "Duplicated parameter host";
        }
    }
    std::string const & Server::getHost() const
    {
        return host;
    }
    void Server::setBody_size_limit(std::string const &body_size_limitt)

    {
        if (body_size_limit == 0)
            body_size_limit = stoi(body_size_limitt);
        else
            throw "Duplicated parameter body_size_limitt";
    }
    int const & Server::getBody_size_limit() const
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