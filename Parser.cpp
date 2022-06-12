#include "Parser.hpp"

    Parser::Parser(int ac, char *path[])
    {
        if (ac == 2)
            checkFile(path[1]);
        else
            throw "ERROR : IN ARGUMENTS!";
        for (int i = servers.size() -1 ; i >= 0; i--)
        {
            std::vector<Location> serverLocations = servers[i].getLocations();
            for (int i = serverLocations.size()  -1 ; i >= 0; i--)
            {
                std::cout << serverLocations[i].getLocation() << std::endl;
            }
            
        }
        

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
            vtmp.clear();
            setLine_noSpace(line);

            switch (stateOfLine())
            {
            case STARTSERV :
                std::cout <<  "server found" << std::endl;
                
                /* code */
                break;
            case ENDSERV :


                servers.push_back(server);
                
                break;
            
            default:
                vtmp = split(line_noSpace);
                if (vtmp.size() != 0)
                {
                    if (vtmp[0] == "server_name")
                        server.setServerName(vtmp[1]);
                    else if (vtmp[0] == "location")
                        location.setLocation(vtmp[1]);
                    else if (vtmp[0] == "]")
                        server.locationADD(location);
                    else if (vtmp[0] == "root")
                        location.setRoot(vtmp[1]);
                    else if (vtmp[0] == "port")
                        server.setPort(vtmp[1]);
                    else if (vtmp[0] == "host")
                        server.setHost(vtmp[1]);
                    else if (vtmp[0] == "body_size_limit")
                        server.setBody_size_limit(vtmp[1]);
                    else if (vtmp[0] == "method")
                        location.setRoot(vtmp[1]);
                    else if (vtmp[0] == "autoindex")
                        location.setAutoindex(vtmp[1]);
                    else if (vtmp[0] == "default")
                        location.setDefaultt(vtmp[1]);
                    else if (vtmp[0] == "upload")
                        location.setUpload(vtmp[1]);
            
                    
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

std::vector< std::string > Parser::split(std::string line)
{
        std::string ret = "";
        std::vector< std::string > v;
        std::size_t found;
        int start = 0;
        found = line.find("=", 0);

        while (found != std::string::npos)
        {
            ret = line.substr(start,  found  - start);
            line.erase(start, found +  1);
            v.push_back(ret);
            found = line.find("=" , found + 1);
        }
        v.push_back(line);
        return (v);
}
