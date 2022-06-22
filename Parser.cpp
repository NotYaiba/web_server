#include "Parser.hpp"
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
Parser::Parser(int ac, char *path[])
{
    if (ac == 2)
        checkFile(path[1]);
    else
        throw "ERROR : IN ARGUMENTS!";
    makeMap();
    // debug();
    // for (std::map<int ,  std::vector < Server > >::iterator i = mapServers.begin(); i != mapServers.end(); i++)
    // {
    //     std::cout << i->first << std::endl;
    // }
    
}

Parser::~Parser()
{
}
void Parser::checkFile(char *path)
{
    std::ifstream fs(path);
    if (fs.is_open() ==  false)
        throw "ERROR : FILE NOT FOUND!";
    readFile(path);
}
void Parser::readFile(char *path)
{
    std::ifstream MyReadFile(path);
    std::vector< std::string > vtmp;
    Server server;
    Location location;
    while (std::getline (MyReadFile, line)) 
    {
        lineS++;
        std::cout << "_line ; "<< lineS.getLine() << std::endl;
        vtmp.clear();
        setLine_noSpace(line);



        switch (stateOfLine())
        {
        case STARTSERV :
            server.clear();
            break;
        case ENDSERV :
            servers.push_back(server);
            break;
        default:
            vtmp = split(line_noSpace, "=");
            if (vtmp.size() != 0)
            {
                if (vtmp[0][0] == '#' || line_noSpace == "")
                    continue;
                else if (vtmp[0] == "server_name")
                    server.addServerName(vtmp[1]);
                else if (vtmp[0] == "location")
                    location.setLocation(vtmp[1]);
                else if (vtmp[0] == "]")
                {
                    server.locationADD(location);
                    location.clear();
                }
                else if (vtmp[0] == "root")
                    location.setRoot(vtmp[1]);
                else if (vtmp[0] == "port")
                    server.setPort(vtmp[1]);
                else if (vtmp[0] == "host")
                    server.setHost(vtmp[1]);
                else if (vtmp[0] == "body_size_limit")
                    server.setBody_size_limit(vtmp[1]);
                else if (vtmp[0] == "method")
                    location.setMethod(vtmp[1]);
                else if (vtmp[0] == "autoindex")
                    location.setAutoindex(vtmp[1]);
                else if (vtmp[0] == "default")
                    location.setDefaultt(vtmp[1]);
                else if (vtmp[0] == "upload")
                    location.setUpload(vtmp[1]);
                else if (vtmp[0] == "error_page")
                    server.setErrorpage(vtmp[1]);
                else if (vtmp[0] == "redirect")
                    location.setRedirect(vtmp[1]);
                else if (vtmp[0] == "cgi")
                {
                        std::vector< std::string > tmp = split(line, "=");
                        std::vector< std::string > tmp2 = split(tmp[1], " ");
                    if (tmp2.size() >= 2 )
                        server.addtoCgiMap(removeSpaces(tmp2[0]),removeSpaces(tmp2[1]) );
                }
                else
                {
                    std::cout << "test :" << line_noSpace << std::endl;
                    throw "error in line !";
                }
                
            }
            
            break;
        }

    }
    MyReadFile.close();


}
void Parser::setLine_noSpace(std::string line) 
{
    line_noSpace = removeSpaces(line);
}
int Parser::stateOfLine()
{
    if (line_noSpace == "server{")
        return (-1);
    else if (line_noSpace == "}")
        return (1);
    else 
        return (0);
} 



std::string removeSpaces(std::string str)
{
str.erase(remove(str.begin(), str.end(), ' '), str.end());
return str;
}


// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

std::vector< std::string > Parser:: split(std::string line, std::string del)
{
    std::string ret = "";
    std::vector< std::string > v;
    std::size_t found;
    int start = 0;
    found = line.find(del, 0);

    while (found != std::string::npos)
    {
        ret = line.substr(start,  found  - start);
        line.erase(start, found +  1);
        ret =trim(ret);
        if (ret != "")
            v.push_back(ret);
        found = line.find(del , found + 1);
    }
    v.push_back(line);
    return (v);
}

std::vector < Server > const & Parser::getServers() const 
{
    return servers;
}

void Parser::makeMap()
{
    for (size_t i = 0 ; i < servers.size(); i++)
    {
        std::vector < Server > tmpserver;
        tmpserver.push_back(servers[i]);
        std::pair<std::map<int ,  std::vector < Server > >::iterator , bool > value = 
        mapServers.insert(std::make_pair(servers[i].getPort(), tmpserver));
        if (value.second == false)
        {
            (value.first)->second.push_back(servers[i]);
        }
        tmpserver.clear();
    }
}

void Parser::debug()
{

        for (std::map<int ,  std::vector < Server > >::iterator i = mapServers.begin(); i != mapServers.end(); i++)
        {
            std::cout << red  <<  i->first << reset <<std::endl;
            std::cout << "----------------------\n";
                std::vector < Server > tmp = i->second;
            for (size_t j = 0 ; j < tmp.size(); j++)
            {
                std::vector< std::string> s = tmp[j].getServerName();
                std::vector< Location> l = tmp[j].getLocations();
                std::cout << blue << s[0] << reset << " | ";
                
                for (size_t k = 0; k < l.size(); k++)
                {
                    std::cout << yellow << l[k].getLocation() << reset << " " ;
                }
                std::cout  << " |" << std::endl;
            }

        }
}

void Parser::checkError()
{
    // if ()
}