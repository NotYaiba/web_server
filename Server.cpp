#include "Server.hpp"

    Server::Server() : port(0), host("") , body_size_limit(10) , error_page("")
    {

    }
    Server::~Server()
    {

    }
    void Server::addServerName(std::string const &servername) 
    {
        if (servername == "")
            throwError(EMPTY, "servername");
        for (size_t i = 0 ; i <  serverNames.size() ; i++)
        {
            if (servername == serverNames[i])
                throwError(DUP, "servername");
        }
        serverNames.push_back(servername);
    }
     std::vector< std::string> const &Server::getServerName() const
    {
        return (serverNames);
    }

    void Server::setPort(std::string const &rot) 
    {
        if (rot == "")
            throwError(EMPTY, "port");
        else if (!isDigit(rot))
            throwError(IVA, "port");
        if (port == 0)
            port = stoi(rot);
        else
            throwError(DUP , "port");
    }
    int const & Server::getPort()const 
    {
        return port;
    }

    void Server::setHost(std::string const &hostt)
    {
        if (hostt == "")
            throwError(EMPTY, "host");
        if (host == "")
            host = hostt;
        else
            throwError(DUP , "host");
    }
    std::string const & Server::getHost() const
    {
        return host;
    }
    void Server::setBody_size_limit(std::string const &body_size_limitt)

    {

        if (!isDigit(body_size_limitt))
            throwError(IVA, "body_size_limit");
        if (body_size_limit == 10)
            body_size_limit = stoi(body_size_limitt);
        else
            throwError(DUP , "body_size_limit");

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

void    Server::debug()
{
    std::cout  <<blue << "serverNames :" << reset  << serverNames[0] << std::endl; 
    std::cout  <<blue << "port :" << reset  << port << std::endl; 
    std::cout  <<blue << "host :" << reset  << host<< std::endl; 
    std::cout  <<blue << "error_page :" << reset  << error_page << std::endl; 
    for (size_t i = 0 ; i < locations.size() ; i++)
        locations[i].debug();
}

void Server::throwError(int type, std::string para)
{
    std::string nb = std::to_string(line);
    nb = "Line : " + nb; 
    std::stringstream errr(nb);
    switch (type)
    {
        case DUP :
            errr << red <<nb <<  reset <<  "  ERROR Duplicated Parameter " << para   ;
            break;
        case EMPTY :
            errr << red <<nb <<  reset <<  "  ERROR Empty Parameter " << para   ;
        case IVA :
            errr << red <<nb <<  reset <<  "  ERROR Invalid Parameter " << para   ;
            break;
        
    }
        throw errr.str();

}