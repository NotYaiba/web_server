#include "Request.hpp"
#include <sstream>
#include "utils/Tools.hpp"

Request::Request()
{
i = 0;
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
        else
            ischuncked = 0; 
        int testoffset = vect[0].size();
        fillBody( buff + testoffset , read - testoffset, _bodyfd);
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

void Request::fillBody( char  *buff, int read, int _bodyfd)
{
    int debug_fd = open("body_debug", O_RDWR | O_CREAT | O_APPEND, 0666);
    std::string tmp(buff, read);
    body  = body +  tmp ;
    checkEndRequest(buff , read);
    write (debug_fd,  buff, read );
    if (ischuncked)
    {
        size_t begin = body.find("\r\n");
        size_t end = body.find("\r\n" , begin + 2);

        while (begin != std::string::npos && end != std::string::npos )
        {
            if ( isChunksize(begin + 2 , end  , body ))
            {
                int i = begin;
                while (i < end)
                {
                    std::cout << body[i];
                    i++;
                } 
                body.erase(begin , end + 2);
                write(_bodyfd , body.c_str() , strlen(body.c_str()) );
                body = "";
                begin = body.find("\r\n");
                end = body.find("\r\n" , begin + 2);
            }
            else
            {
                body.erase(0, end + 2);
                begin = body.find("\r\n");
                end = body.find("\r\n" , begin + 2);
            }
        }
        if (begin == std::string::npos && end == std::string::npos){
            write(_bodyfd , body.c_str() , strlen(body.c_str()) );
            body = "";
            return ;
        }
        else if (begin != std::string::npos && end == std::string::npos){
            std::cout << "salaaaaaaaam\n";
            write(_bodyfd , body.c_str() , begin );
            body.erase(0, begin);
            return ;
        }
    }   
    else
        write (_bodyfd,  buff, read );  
   
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
            headers.insert(std::make_pair(vect3[0], trim(value)));
        }
        i++;
    }
}


bool Request::checkEndRequest( char const *buff, int read)
{
    if (ischuncked )
    {
         int i = 0;
        while (i < read)
        {
            if   (i < read && buff[i] == '\r' && buff[i + 1] == '\n'  &&  buff[i + 2] == '0' && buff[i + 3] == '\r'  && buff[i + 4] == '\n'  )
            {
                std::cout << "ENDD OF REQUEST\n";
                isslastRead = true;
                isFrstRead = true;
                    return true;

            }
            i++;
        } 
        return false;
    }
        return false;
  

}

bool Request::isChunksize(size_t begin , size_t end, std::string str)
{
    int i = 1;
    int expetedSize = end - begin;
    
    while (isxdigit(str[begin + i]) )
        i++;
    std::cout<<  "i :"<< i << std::endl;
    std::cout<<  "expected :"<< expetedSize << std::endl;
    if (i == expetedSize)
        return true;
    return false;
}