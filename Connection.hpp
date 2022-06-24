#ifndef  CONNECTION_HPP
#define  CONNECTION_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "./utils/Tools.hpp"
#include <iomanip>
#include <sstream>
#include <map>
#include "Socket.hpp"

class Connection 
{    
    std::map < int ,  std::vector < Server > > mapServersP;
    std::map < int ,  std::vector < Server > > mapServersF;
    std::map < int, int > fd_map;

    public:
    Connection( std::map < int ,  std::vector < Server > > msP);
    Connection( Connection const & ref);
    Connection();
    Connection const & operator=(Connection const & ref);
    ~Connection();
    void debug ();
    void createConnnections ();
    void makeFDMap ();
    std::map < int ,  std::vector < Server > >const & getMapServersP() const;
    std::map < int ,  std::vector < Server > >const & getMapServersF() const;
     std::map < int, int > const &                    getFd_map() const;
    void throwError(int type, std::string para);
};

#endif
