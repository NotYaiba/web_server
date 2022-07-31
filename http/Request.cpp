#include "Request.hpp"


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
    content_type = "";
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
        std::cout << "read12 : " << read << std::endl;
    std::cerr << buff << std::endl;
    if (isFrstRead)
    {
        std::cout << "firstread :"<< (read) << std::endl;

        vect = split(str, "\r\n\r\n");
        std::cout << "under spli :"<< (read) << std::endl;
        fillHeaders( vect[0]);
        std::cout << "under fillHeaders :"<< (read) << std::endl;
        checkHeaders();
        std::cout << "under checkHeaders :"<< (read) << std::endl;
        createFile();
        std::cout << "create :"<< (read) << std::endl;
        int testoffset = vect[0].size() + 4 ;
        std::cout << "read - test "<< (read - testoffset) << std::endl;
        fillBody( buff + testoffset , read - testoffset, _bodyfd);
        std::cout << "read123 : " << read << std::endl;

        isFrstRead = false;
    }
    else
    {
        std::cout << "read1234 : " << read << std::endl;

        fillBody( buff, read, _bodyfd);
    }
   
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
        std::cout << "read12345 : " << read << std::endl;

    fill_vect(buff, read);

    if (ischuncked == 1)
    {
        if (findchunkSize())
        {
            writeVect(_bodyfd);
        }
    }   
    else 
    {
        
        write (_bodyfd,  buff, read );  
    }

    checkEndRequest(buff , read);
}


void Request::fillMethod()
{
    // std::cout << Muv << std::endl;

    std::vector<std::string> s  = split(Muv, " ");
    method = s[0];
    Uri = fixIt(s[1]);
    std::cout << yellow << method << std::endl;
    std::cout << yellow << Uri << std::endl;
    if (method != "POST" || method != "GET" || method != "DELETE")
        invalidMethod = -1;
    
}

void Request::fillHeaders(std::string header)
{
    std::vector<std::string> vect3;
    std::string line;
    std::stringstream headerStream(header);
    std::string value;
    int i = 0;
    std::cout <<" header befor " << header << std::endl;
    while (std::getline (headerStream, line))
    {
        std::cout << "header : " << line << std::endl;
        if (i == 0)
            Muv  = line;
        else
        {
            vect3 = split(line, ":");
            value = vect3[1];
            if (vect3.size() > 2)
            {

                size_t k  = 2;
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
    
    
    if (ischuncked == 1 )
    {
        // std::cout << yellow << "request id chunked !\n" << reset ;
         int i = 0;
        while (i < read)
        {
            if   (i  +  4< read && buff[i] == '\r' && buff[i + 1] == '\n'  &&  buff[i + 2] == '0' && buff[i + 3] == '\r'  && buff[i + 4] == '\n'  )
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
        if (content_length == 0)
        {
            /* code */
            std::cout << "ENDD OF REQUEST\n";
            
            isslastRead = true;
            isFrstRead = true;
            return true;
        }
        FILE *p_file = NULL;
        std::string tmp = "./tmp/" + body;
        
        p_file = fopen(tmp.c_str(),"rb");
        fseek(p_file,0,SEEK_END);
        int size = ftell(p_file);
        fclose(p_file);


        if (content_length <= (size_t)size)
        {
                isslastRead = true;
                isFrstRead = true;
                return true;
        }
    }  
     return false;
  

}



void Request::fill_vect(char *buff , int read)
{
    std::cout << "read : " << read << std::endl;
    for (int i = 0 ; i < read; i++)
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
                size_t n = i  + 2;
                while  (n  <  god_vect.size()  &&  isxdigit(god_vect[n]) )
                    n++;
                if (n + 1 < god_vect.size() &&   god_vect[n] == '\r' && god_vect[n + 1] == '\n')
                {
                    dd = 1;
                    god_vect.erase(god_vect.begin() + i, god_vect.begin() + n + 2);
                }
            }
        }    
    }
    if (dd == 1)
        return true;
    return false;
}
void  Request::createFile()
{

    std::map<std::string, std::string>::iterator it =  headers.find("Content-Type");
    if (it != headers.end())
        content_type = it->second;
    body += random_string() + get_file_ext(content_type);
    std::string tmp = "./tmp/" + body;
    _bodyfd = open(tmp.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666);
}
void   Request::checkHeaders()
{
    fillMethod();
    std::map<std::string, std::string>::iterator it;
    it = headers.find("Content-Length");
    if (it != headers.end())
    {
    content_length = stoi(it->second);
    std::cout << blue << "=============>"<< content_length << reset<< std::endl;
    std::cout << blue << "=============>"<< ischuncked << reset<< std::endl;
    
    }

    it =  headers.find("Transfer-Encoding");
    if (it != headers.end())
        ischuncked = (it ->second == "chunked") ? 1  : 0;
    else if (ischuncked == 0)
        status_code = 501;
    else if (ischuncked == -1 && content_length == 0 && method == "POST")
        status_code = 400;
    else if (Uri.size() > 2048)
        status_code = 404;
    
}

void  Request::InitData()
{
    close(_bodyfd);
    this->isFrstRead = true;
    this->isslastRead =  false;
    this->ischuncked = -1;
    this->invalidMethod = 0;
    this->Uri = "";
    this->Muv = "";
    this->method = "";
    this->content_length = 0;
    body = "";
    content_type = "";
    headers.clear();
    god_vect.clear();
    god_vect.push_back('\r');
    god_vect.push_back('\n');
}
Request &Request::operator=(Request  const & src)
{
    status_code = src.getStatusCode();
    Uri = src.getUri();
    method = src.getMethod();
    body = src.getBody();
    headers = src.getHeaders();
    return *this;
}