#ifndef  LOCATION_HPP
#define  LOCATION_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "./utils/Tools.hpp"
#include <iomanip>
#include <sstream>


class Location 
{    
    std::string location;
    std::string method;
    std::string root;
    bool autoindex;
    std::string defaultt;
    std::string upload;
    std::string redirect;
    int line;
    public: 
    void setLine(int const & linne)
    {
        line = linne;
    }
    Location() ;
    ~Location();
    void setLocation(std::string const &rot);
    std::string const & getLocation()const ;

    void setMethod(std::string const &rot);
    std::string const & getMethod()const ;

    void setRoot(std::string const &rot);
    std::string const & getRoot()const ;

    void setAutoindex(std::string const &rot);
    bool  getAutoindex()const ;

    void setDefaultt(std::string const &rot);
    std::string const & getDefaultt()const ;

    void setUpload(std::string const &rot);
    std::string const & getUpload()const ;
    void setRedirect(std::string const &rot);
    std::string const & getRedirect()const ;
    void clear()
    {
        location = "" ;
        method = "";
        root = "" ;
        defaultt = "";
        upload = "";
        redirect = "";
    }
    void debug ();
    void throwError(int type, std::string para);
    Location &operator=(Location  const & src);
    Location(Location  const & src);

};


#endif
