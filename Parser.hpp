#ifndef  PARSER_HPP
#define  PARSER_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include "Server.hpp"
#include <fcntl.h>

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
    void setLine_noSpace(std::string line) ;
    void checkError() ;
    int stateOfLine();
    std::vector< std::string > split(std::string line, std::string del);
    void makeMap();
    void debug();

};
std::string removeSpaces(std::string str);
   

#endif
