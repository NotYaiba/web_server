#include "Request.hpp"
#include <sstream>
#include "utils/Tools.hpp"

Request::Request()
{
    i = 0;
    this->isFrstRead = true;
    this->isslastRead =  false;
    this->ischuncked = -1;
    this->invalidMethod = 0;
    this->Uri = "";
    this->Muv = "";
    this->content_length = 0;
    body = "";
    god_vect.push_back('\r');
    god_vect.push_back('\n');
    method_vect.push_back("POST");
    method_vect.push_back("DELETE");
    method_vect.push_back("GET");
    j = 0;
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
            ischuncked = (it ->second == "chunked") ? 1  : 0; 
        else
            ischuncked = 0; 
        int testoffset = vect[0].size() + 4;
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
    checkEndRequest(buff , read);
    fill_vect(buff, read);
    if (ischuncked)
    {
        if (findchunkSize())
        {
            writeVect(_bodyfd);
        }
    }   
    else
        write (_bodyfd,  buff, read );  
    int debug_fd = open("body_debug", O_RDWR | O_CREAT | O_APPEND, 0666);
    write (debug_fd,  buff, read );
   
}

// std::vector<char> fillv()
// {
//     std::vector<char> v;
//     char c = 'A';
//     for (; c <= 'Z'; c++)
//         v.push_back(c);
//     c = 'a';
//     for (; c <= 'z'; c++)
//         v.push_back(c);
//     c = '1';
//     for (; c <= '9'; c++)
//         v.push_back(c);
//     c = '#';
//     for (; c <= '/'; c++)
//         v.push_back(c);
//     v.push_back('!');
//     v.push_back(':');
//     v.push_back(';');
//     v.push_back('=');
//     v.push_back('?');
//     v.push_back('@');
//     v.push_back('[');
//     v.push_back(']');
//     v.push_back('~');
//     return (v);
// }

// void validUri(std::string s, int invalidUri)
// {

//     std::vector<char> v = fillv();
//     for (int i = 0; i < s.size(); i++)
//     {
//         for (int k = 0; k < v.size(); k++)
//         {
//             if (s[i] == v[k])
//             {
//                 invalidUri = 1;
//                 break;
//             }
//             continue;
//         }
//     }
//     std::cout << invalidUri << std::endl;

// }

void Request::fillMethod()
{
    std::vector<std::string> s  = split(Muv, "/");
    for (int i = 0; i < method_vect.size(); i++)
    {                
        if (s[0] == method_vect[i])
            j = 1;
        continue;
    }
    if (j != 1)
        invalidMethod = -1;
    s = split(s[1], "/");
    Uri = s[0];
    // std::cout << Uri << std::endl;
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
        {
            Muv  = line;
            fillMethod();
        }
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
            std::map<std::string, std::string>::iterator it;
            it = headers.find("Content-Length");
            if (it != headers.end())
                content_length = atoi(((*it).second).c_str());
            // std::cout << content_length << std::endl;
            // std::cout << "---------\n";
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
    else
    {
        std::ifstream testFile("./body", std::ios::binary);
        testFile.lseek(0, std::ios::end);
        int file_size = testFile.tellg();
        std::cout<< file_size<< " file_size\n";
        std::cout<< content_length<< " content lenghnt\n";
    
        if (content_length <= file_size)
        {
                std::cout << "heey\n";
                isslastRead = true;
                isFrstRead = true;
                return true;
        }
    }  
     return false;
  

}

bool Request::isChunksize(size_t begin , size_t end, char * str)
{
    int i = 1;
    int expetedSize = end - begin;
    
    while (isxdigit(str[begin + i]) )
    {
        std::cout << str[begin + i];
        i++;
    }
    // std::cout<<  "i :"<< i << std::endl;
    // std::cout<<  "expected :"<< expetedSize << std::endl;
    if (i == expetedSize)
        return true;
    return false;
}


void Request::fill_vect(char *buff , size_t read)
{
    for (size_t i = 0 ; i < read; i++)
    {
        god_vect.push_back(buff[i]);
    }
}
void Request::writeVect(int fd)
{
    for (size_t i = 0 ; i < god_vect.size(); i++)
    {
        write(fd, &god_vect[i], 1);
    }
    god_vect.clear();
}
bool Request::findchunkSize()
{
    int dd = 0;
    for (size_t i = 0 ; i < god_vect.size(); i++)
    {
        if (god_vect[i] == '\r')
        {

            if (i + 1 <  god_vect.size() && god_vect[i + 1] == '\n')
            {
                int n = i  + 2;
                while  (n  <  god_vect.size()  &&  isxdigit(god_vect[n]) )
                {
                    n++;
                }

                if (n + 1 < god_vect.size() &&   god_vect[n] == '\r' && god_vect[n + 1] == '\n')
                {
                    dd = 1;
                    // std::cout << "mseeh l9lawi\n";
                    size_t tmp  = i;
                    for (size_t i = tmp; i < n+2; i++)
                    {
                    //    std::cout << god_vect[i];
                    }
                    // std::cout << n + 1 - i<< std::endl;
                    god_vect.erase(god_vect.begin() + i, god_vect.begin() + n + 2);
                }
            }
        }
           
    }
    if (dd == 1)
        return true;
    return false;
}