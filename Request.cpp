#include "Request.hpp"
#include <sstream>
#include "utils/Tools.hpp"

Request::Request()
{

    this->isFrstRead = true;
    this->isslastRead =  false;
}

Request::~Request()
{
    
}

void    Request::fillRequest(char *buff, int read)
{
    int     i = 0;
    std::vector<std::string> vect;
    std::vector<std::string> vect2;
    std::vector<std::string> vect3;
    std::string str = buff;
    _bodyfd = open("body", O_RDWR | O_CREAT | O_APPEND, 0666);
    
    if (isFrstRead)
    {
        vect = split(str, "\r\n\r\n");
        // std::cout << vect[0];
        fillHeaders(vect[0]);
        write (_bodyfd, vect[1].c_str(), vect[1].size() );
    }
    else
    {   
        // std::cout << "salam\n";
        write (_bodyfd, str.c_str(), str.size() );

        size_t found = str.find("0\r\n");
        if (found != std::string::npos)
            isslastRead = true;
    }
    // debug();
    isFrstRead = false;
}

void    Request::debug()
{
    std::cout  <<blue << "Muv :" <<  reset << Muv << std::endl; 

    // std::cou<<t << blue << "cgimap :" << reset << setw(50) << defaultt <<  std::endl; 
    for (std::map<std::string, std::string>::iterator  i =  headers.begin() ; i != headers.end() ; i++)
    {
        std::cout  <<yellow<< (i->first) << reset <<" : " << (i->second)  << std::endl;
    }
}

void Request::fillHeaders(std::string header)
{
    std::vector<std::string> vect3;
    std::string line;
    std::stringstream headerStream(header);
    std::string value;
    int i = 0;
    while (std::getline (headerStream, line))
    {
    
        if (i == 0)
            Muv  = line;
        else
        {
            vect3 = split(line, ":");
            value = vect3[1];
            if (vect3.size() > 2)
            {
                int k  = 2;
                while (k < vect3.size())
                    value +=  + ":" + vect3[k++];
            }   
            headers.insert(std::make_pair(vect3[0], value));
        }
        i++;
    }
}
