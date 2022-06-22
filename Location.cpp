#include "Location.hpp"

    Location::Location(): location("") , method(""), root("") , defaultt("") , upload("") , redirect("")
    {

    }
    Location::~Location()
    {

    }

    
    void Location::setLocation(std::string const &rot)
    {
        if (location == "")
            location = rot;
        else
            throw "multiple parameter location!";
    }
    std::string const & Location::getLocation()const 
    {
        return location;
    }
    void Location::setMethod(std::string const &rot)
    {
        if (method == "")
            method = rot;
        else
            throw "multiple parameter method!";
    }
    std::string const & Location::getMethod()const 
    {
        return method;
    }

    void Location::setAutoindex(std::string const &rot)
    {
        if (rot == "on")    
            autoindex = true;
        else if (rot == "off")    
            autoindex = false;
        else
            throw "Invalid Auto Index !";

    }
    bool Location::getAutoindex()const 
    {
        return autoindex;
    }

    void Location::setDefaultt(std::string const &rot)
    {
        if (defaultt == "")
            defaultt = rot;
        else
            throw "multiple parameter defaultt!";
 
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

        if (root == "")
            root = rot;
        else
        {
            throw "multiple parameter root  !";
        }
    }
    void Location::setRedirect(std::string const &rot)
    {

        if (redirect == "")
            redirect = rot;
        else
        {
            throw "multiple parameter redirect  !";
        }
    }
    std::string const & Location::getRedirect()const 
    {
        return redirect;
    }
    std::string const & Location::getRoot()const 
    {
        return root;
    }


void    Location::debug()
{
    std::cout <<"--------------------------------------------------------" << std::endl; 
    std::cout << std::setw(5) << red << "location :" << reset  << location << std::endl; 
    std::cout << std::setw(5) << red << "method :" << reset  << method << std::endl; 
    std::cout << std::setw(5) << red << "root :" << reset  << root<< std::endl; 
    std::cout << std::setw(5) << red << "autoindex :" << reset  << autoindex << std::endl; 
    std::cout << std::setw(5) << red << "defaultt :" << reset  << defaultt << std::endl; 
    std::cout << std::setw(5) << red << "upload :" << reset  << upload << std::endl; 
    std::cout <<"--------------------------------------------------------" << std::endl; 
}