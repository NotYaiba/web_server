#include "Request.hpp"
#include <sstream>
#include "utils/Tools.hpp"

Request::Request()
{

    this->isFrstRead = true;
    this->isslastRead =  false;
    this->ischuncked = -1;
}

Request::~Request()
{
    
}

void    Request::fillRequest(char *buff, int read)
{
    std::vector<std::string> vect;
    std::string str(buff, read);
    _bodyfd = open("body", O_RDWR | O_CREAT | O_APPEND, 0666);
    if (isFrstRead)
    {
        vect = split(str, "\r\n\r\n");
        fillHeaders( vect[0]);
        std::map<std::string, std::string>::iterator it =  headers.find("Transfer-Encoding");
        if (it != headers.end())
        {
            ischuncked = (it ->second == "chunked") ? 1  : 0; 
        }
        write (_bodyfd, vect[1].c_str(), strlen(vect[1].c_str()) );
        isFrstRead = false;
    }
    else
        fillBody( buff, read, _bodyfd);
    // debug();
}

void    Request::debug()
{
    std::cout  <<blue << "Muv :" <<  reset << Muv << std::endl; 
    // std::cou<<t << blue << "cgimap :" << reset << setw(50) << defaultt <<  std::endl; 
    for (std::map<std::string, std::string>::iterator  i =  headers.begin() ; i != headers.end() ; i++)
    {
        std::cout  <<yellow<< "[" <<(i->first) << "}" << reset <<" : {"  << (i->second) << "}"   << std::endl;
    }
}

void Request::fillHeaders(std::string header)
{
    std::cout << header << std::endl;
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
            headers.insert(std::make_pair(vect3[0], trim(value)));
        }
        i++;
    }
}

void Request::fillBody( char *buff, int read, int _bodyfd)
{
    if (ischuncked)
    {
    }
    else
    {   
        write (_bodyfd,  buff, read );  
        int i = 0;
        while (i < read)
        {
            if (buff[i] == '0' && buff[i + 1] == '\r'  && buff[i + 2] == '\n'  )
            {
                std::cout << "ENDD OF REQUEST\n";
                isslastRead = true;
                isFrstRead = true;
            }
            i++;
        } 
    }
}
