#include "Response.hpp"

Response::Response(Server  serv , Request req)
{
    _server = serv;
    _req  = req;
    statusCode =  req.getStatusCode();
    if (statusCode != 200 ||statusCode != 201)
    {
             // todo send response with status code; 
    }
    validMethod = 0;
    findMatchingLocation();

}



void   Response::findMatchingLocation()
{
    getLocation();
    getMethod();
}

void Response::getMethod()
{
    std::vector <std::string> v = split(_loc.getMethod(), " ");
    for (size_t i = 0 ; v.size() ; i++)
    {
        if (_req.getMethod() == v[i])
            validMethod = 1;
    }
    std::cout <<validMethod << std::endl;

}

void Response::getLocation()
{
    std::vector<Location> v = _server.getLocations();
    for (int i = 0; i < v.size(); i++)
       locations.push_back(v[i]);
    for (int i = 0; i < locations.size(); i++)
    {
        if (_loc.getLocation().size() < locations[i].getLocation().size())
           _loc = locations[i];
    }
}
