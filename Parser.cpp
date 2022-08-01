#include "Parser.hpp"

Parser::Parser(int ac, char *path[])
{
    if (ac == 2)
        checkFile(path[1]);
    else
        throw "ERROR : IN ARGUMENTS!";
    makeMap();
}

Parser::~Parser()
{
}
void Parser::checkFile(char *path)
{
    std::ifstream fs(path);
    if (fs.is_open() ==  false)
    {
        fs.close();
        throw "ERROR : FILE NOT FOUND!";
    }
    fs.close();
    readFile(path);
}
void Parser::readFile(char *path)
{
    bool isServ = false;
    bool isLoc = false;
    std::ifstream MyReadFile(path);
    std::vector< std::string > vtmp;
    std::vector< std::string > vtmpSpace;
    Server server;
    Location location;
    int lineNb = 0;
    while (std::getline (MyReadFile, line)) 
    {
        lineNb++;
        server.setLine(lineNb);
        location.setLine(lineNb);
        vtmp.clear();
        setLine_noSpace(line);
        int n = stateOfLine();
        switch (n)
        {
        case STARTSERV :
            if (isServ == true)
                throwError(SYN, "" , lineNb);
            server.clear();
            isServ = true;
            break;
        case ENDSERV :
            checkServer(server, lineNb);
            if (isServ == false)
                throwError(SYN, "" , lineNb);
            servers.push_back(server);
            isServ = false;
            break;

        default:
            vtmp = split(line_noSpace, "=");
            vtmpSpace = split(line, "=");
            if (vtmp.size() != 0)
            {
                if (vtmp[0][0] == '#' || line_noSpace == "")
                    continue;
                else if (vtmp[0] == "server_name")
                    server.addServerName(vtmp[1]);
                else if (vtmp[0] == "location")
                {
                    if (isLoc == true)
                        throwError(SYN, "" , lineNb);
                    isLoc = true;
                    location.setLocation(vtmp[1]);
                }
                else if (vtmp[0] == "]")
                {
                    if (isLoc == false)
                        throwError(SYN, "" , lineNb);
                    isLoc = false;
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
                    location.setMethod(vtmpSpace[1]);
                else if (vtmp[0] == "autoindex")
                    location.setAutoindex(vtmp[1]);
                else if (vtmp[0] == "default")
                    location.setDefaultt(vtmp[1]);
                else if (vtmp[0] == "upload")
                    location.setUpload(vtmp[1]);
                else if (vtmp[0] == "error_page")
                    server.setErrorpage(vtmp[1]);
                else if (vtmp[0] == "redirect")
                    location.setRedirect(vtmpSpace[1]);
                else if (vtmp[0] == "cgi")
                {
                        std::vector< std::string > tmp = split(line, "=");
                        std::vector< std::string > tmp2 = split(tmp[1], " ");
                    if (tmp2.size() >= 2 )
                        location.addtoCgiMap(removeSpaces(tmp2[0]),removeSpaces(tmp2[1]) );
                }
                else
                {
                   throwError(SYN, "" , lineNb);
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
            if (servers[i].getServerName()[0] == mapServers[servers[i].getPort()][0].getServerName()[0]  )
                throwError(IVA, "duplicated Servers", 0);
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

void Parser::throwError(int type, std::string para , int lineNB)
{
    std::string nb = std::to_string(lineNB);
    nb = "Line : " + nb; 
    std::stringstream errr(nb);
    switch (type)
    {
        case DUP :
            errr << red <<nb <<  reset <<  "  ERROR Duplicated Parameter " << para   ;
            break;
        case EMPTY :
            errr << red <<nb <<  reset <<  "  ERROR Empty Parameter " << para   ;
        case IVA :
            errr << red <<nb <<  reset <<  "  ERROR Invalid Parameter " << para   ;
            break;
        case SYN :
            errr << red <<nb <<  reset <<  "  ERROR Invalid  Syntax " << para   ;
            break;
        
    }
        throw errr.str();

}

std::vector < Server > const & Parser::getServers() const 
{
    return servers;
}

std::map < int ,  std::vector < Server > >const & Parser::getMapServers()const 
{
    return (mapServers);
}

void Parser::checkServer(Server  s , int line)
{
   
        std::vector<Location> locations = s.getLocations();
        for (size_t i = 0; i < locations.size(); i++)
        {
            std::map<std::string , std::string> cgi = locations[i].getCgiMap();
            if(locations[i].getUpload() != "" && cgi.size() != 0 )
            {
                throwError(IVA, "Cgi and upload in the same location." , line);
            }
        }
    
}