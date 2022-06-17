#include "Location.hpp"

    Location::Location() 
    {

    }
    Location::~Location()
    {

    }

    
    void Location::setLocation(std::string const &rot)
    {
        location = rot;
    }
    std::string const & Location::getLocation()const 
    {
        return location;
    }
    void Location::setMethod(std::string const &rot)
    {
        method = rot;
    }
    std::string const & Location::getMethod()const 
    {
        return method;
    }

    void Location::setAutoindex(std::string const &rot)
    {
        autoindex = rot;

    }
    std::string const & Location::getAutoindex()const 
    {
        return autoindex;
    }

    void Location::setDefaultt(std::string const &rot)
    {
        defaultt = rot;
 
    }
    std::string const & Location::getDefaultt()const 
    {
        return defaultt;
    }

    void Location::setUpload(std::string const &rot)
    {
        upload = rot;
 
    }
    std::string const & Location::getUpload()const 
    {
        return upload;
    }
    void Location::setRoot(std::string const &rot)
    {
        root = rot;
    }
    std::string const & Location::getRoot()const 
    {
        return root;
    }

