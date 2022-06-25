#include "Request.hpp"
#include <sstream>
#include "utils/Tools.hpp"

Request::Request()
{

    this->isFrstRead = true;
    this->isslastRead =  false;
    this->ischuncked = -1;
    body = "";
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
        fillBody( vect[1].c_str() , strlen(vect[1].c_str()), _bodyfd);
        // write (_bodyfd, vect[1].c_str(), strlen(vect[1].c_str()) );
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

void Request::fillBody( char const *buff, int read, int _bodyfd)
{
    body +=  buff;
     write (_bodyfd,  buff, read );  

    // if (ischuncked)
    // {
    //     size_t found = body.find("\r\n");
    //     // std::cout << "kayn " <<found << std::endl;
    //     int i = 2;
    //     // std::cout << "kayn |" << body[found + 2]<< "|" << std::endl;

    //     if (found != std::string::npos)
    //     {
    //         while (isxdigit(body[found + i]))
    //         {
    //             // std::cout << "kayn=> " << body[found + i] << std::endl;
    //             i++;
    //         }
    //         body.erase(found, found + i + 2);
    //         write (_bodyfd,  body.c_str(), strlen(body.c_str()) );  
    //         body = "";
    //     }
    // }
    // else
    // {   
    //     write (_bodyfd,  buff, read );  
    //     int i = 0;
    //     while (i < read)
    //     {
    //         if (buff[i] == '0' && buff[i + 1] == '\r'  && buff[i + 2] == '\n'  )
    //         {
    //             std::cout << "ENDD OF REQUEST\n";
    //             isslastRead = true;
    //             isFrstRead = true;
    //         }
    //         i++;
    //     } 
    // }
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

