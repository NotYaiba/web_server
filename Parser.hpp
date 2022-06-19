#ifndef  PARSER_HPP
#define  PARSER_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "Server.hpp"

#define  STARTSERV -1
#define  ENDSERV   1

class Parser 
{    
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
    int stateOfLine();
    std::vector< std::string > split(std::string line, std::string del);

};
std::string removeSpaces(std::string str);

#endif
