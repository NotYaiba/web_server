#ifndef  PARSER_HPP
#define  PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Server.hpp"
#include <fcntl.h>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "./utils/Tools.hpp"
#define  STARTSERV -1
#define  ENDSERV   1

class Server;

class Parser 
{    
    
    std::map < int ,  std::vector < Server > > mapServers;
    std::vector < Server > servers;
    std::string line;
    std::string line_noSpace;
    void checkFile(char *path);
    void readFile(char *path);
public: 
    Parser(int ac, char *path[]);
    ~Parser();
    std::vector < Server > const & getServers()const ;
    std::map < int ,  std::vector < Server > >const & getMapServers()const ;
    void setLine_noSpace(std::string line) ;
    void checkError() ;
    int stateOfLine();
    void makeMap();
    void debug();
    void throwError(int type, std::string para , int lineNB);
    
};
   

#endif
