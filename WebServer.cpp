#include "WebServer.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "Response.hpp"
Webserver::Webserver(Connection const &connection)
{
    InitData(connection);
    
}
Webserver::~Webserver()
{
    
}

void  Webserver::InitData(Connection const &connection)
{
    servers = connection.getMapServersF();

    _cnx = connection;
    fd_map = _cnx.getFd_map();
    FD_ZERO(&readset); 
    FD_ZERO(&writeset); 
    FD_ZERO(&readcopy); 
    FD_ZERO(&writecopy);   
    for (std::map < int, int >::iterator it = fd_map.begin(); it != fd_map.end(); it++)
    {
            FD_SET(it->first, &readset);
            Request req;
            req_map.insert(std::make_pair(it->first,req));
    }
    maxfd = (--fd_map.end())->first ;
    readcopy = readset;
}

void Webserver::RunWebServer()
{
    std::cout << red << "MY SERVER IS LISTING!" << reset << std::endl;
    for (;;)
    {
        readset = readcopy;
        writeset = writecopy;
        int rs = select(maxfd + 1 , &readset, &writeset, NULL, NULL);
        if (rs == -1)
            throw "errooor";
        for (int fd = 0; fd <= maxfd; fd++)
        {
            if (FD_ISSET(fd, &readset) || FD_ISSET(fd, &writeset))
            {
                
                if (FD_ISSET(fd, &readset))
                {
                    if (fd_map[fd] == 0)
                        NewConnectionRead(fd);
                    else
                    {
                        //todo set fd to be wrotable
                        HandleRequest(fd);
                    }
                }
                else if (FD_ISSET(fd, &writeset))
                {
                    HandleResponse(fd);
                }
            }
        }
    } 
}

void Webserver::NewConnectionRead(int fd)
{
    int new_fd = Socket::AccectSock(fd);
    fcntl(new_fd, F_SETFL,  O_NONBLOCK);
    std::cout << red  << "|"<< new_fd <<  "|" << reset << std::endl;
    FD_SET(new_fd, &readcopy);
    fd_map.insert(std::make_pair(new_fd, 1));
    servers.insert(std::make_pair(new_fd, servers[fd]));
    Request req;
    req_map.insert(std::make_pair(new_fd ,req));
    fd_map[new_fd] = 1;
    if (new_fd > maxfd)
        maxfd = new_fd;
}

void Webserver::HandleRequest(int fd)
{
    char buf[BUFFER_SIZE];
    int allread  = 0;

    int rb = read(fd,buf,BUFFER_SIZE );

    if (rb == -1)
    {
        std::cout << std::strerror( errno)  << std::endl;
        throw "mochkila\n";
    }
    else if (rb > 0)
    {
        Request req;
        std::map<int , Request>::iterator it = req_map.find(fd);
        
        it->second.fillRequest(buf, rb);

        if ( it->second.isslastRead == true)
        {
           
            FD_SET(fd, &writecopy);
            FD_CLR(fd, &readcopy);
        }
    }
    else if (rb == 0)
    {
        close(fd);
        FD_CLR(fd ,&readcopy);
    }
}


void Webserver::HandleResponse(int fd)
{
    // std::vector <Server> serv =  servers[fd];
    // find if response exists in this fd
    // if true | written bytes | path to file => open file, lseek ritten bytes, close fd, compare written bytes with file size (if written bytes == file size) erase from map 
    // if false construct new resonse || compare written bytes with file size (if written bytes == file size)
    Response res(servers[fd][0], req_map[fd]);
    req_map[fd].InitData();
    char *tello = (char *)("HTTP/1.1 200 OK\nContent-length: 17\n\r\nTello from server");
   
    int returnWrite = write(fd,  res.gethadak().c_str(),  res.gethadak().size());
    FD_CLR(fd, &writecopy);
    close(fd);
}

