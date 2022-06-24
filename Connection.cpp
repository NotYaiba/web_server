#include "Connection.hpp"

    Connection::Connection()
    {

    }
    Connection::Connection( std::map < int ,  std::vector < Server > > msP)
    {
        mapServersP = msP;
        createConnnections();
    }
    Connection::Connection( Connection const & ref)
    {
        *this  =  ref;
    }
    Connection const & Connection::operator=(Connection const & ref)
    {
        fd_map = ref.getFd_map();
        mapServersF =  ref.getMapServersF();
        mapServersP =  ref.getMapServersP();
        return (*this);
    }
    Connection::~Connection()
    {

    }

    void Connection::createConnnections ()
    {
        for (std::map<int ,  std::vector < Server > >::iterator i = mapServersP.begin(); i != mapServersP.end(); i++)
        {
            Socket tmp(i->first);
            fd_map.insert(std::make_pair(tmp.getFd(), 0));
            mapServersF.insert(std::make_pair(tmp.getFd(), i->second));
        } 
    }
     std::map < int ,  std::vector < Server > >const & Connection::getMapServersP() const
     {
        return mapServersP;
     }
    std::map < int ,  std::vector < Server > > const & Connection::getMapServersF() const
    {
        return mapServersF;
    }
    
    std::map < int, int > const & Connection::getFd_map() const
    {
        return  fd_map;
    }
    
    void Connection::debug ()
    {

    }

