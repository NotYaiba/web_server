#include "Request.hpp"
#include "utils/Tools.hpp"

Request::Request()
{

    this->isFrstRead = true;
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
     std::ofstream _body("testingbody");
    if (isFrstRead)
    {
        vect = split(str, "\r\n\r\n");
        fillHeaders(vect);
        _body << vect[1];
    }
    else
    {
        // _bode << buff;
    }
    debug();
    _body.close();
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

void Request::fillHeaders(std::vector<std::string> vect)
{
    std::vector<std::string> vect2;
    std::vector<std::string> vect3;
     for (int i = 0; i < vect.size(); i++)
    {
        vect2 = split(vect[0], "\r\n");
        for (int i = 0; i < vect2.size(); i++)
        {
            if (i == 0)
                Muv  = vect2[i];
            else
            {
                vect3 = split(vect2[i], ":");
                headers.insert(std::make_pair(vect3[0], vect3[1]));
            }
        }
    }
}
