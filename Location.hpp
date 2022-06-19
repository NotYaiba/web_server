#ifndef  LOCATION_HPP
#define  LOCATION_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "./utils/Tools.hpp"
#include <iomanip>


class Location 
{    
    std::string location;
    std::string method;
    std::string root;
    std::string autoindex;
    std::string defaultt;
    std::string upload;
public: 
    Location() ;
    ~Location();
    void setLocation(std::string const &rot);
    std::string const & getLocation()const ;

    void setMethod(std::string const &rot);
    std::string const & getMethod()const ;

    void setRoot(std::string const &rot);
    std::string const & getRoot()const ;

    void setAutoindex(std::string const &rot);
    std::string const & getAutoindex()const ;

    void setDefaultt(std::string const &rot);
    std::string const & getDefaultt()const ;

    void setUpload(std::string const &rot);
    std::string const & getUpload()const ;
    void debug ();


};

#endif
