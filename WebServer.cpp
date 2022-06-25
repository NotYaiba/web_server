#include "WebServer.hpp"

Webserver::Webserver(Connection const &connection)
{
    InitData(connection);
    
}
Webserver::~Webserver()
{
    
}

void  Webserver::InitData(Connection const &connection)
{
    _cnx = connection;
    fd_map = _cnx.getFd_map();
    FD_ZERO(&readset); 
    FD_ZERO(&writeset); 
    FD_ZERO(&readcopy); 
    FD_ZERO(&writecopy);   
    for (std::map < int, int >::iterator it = fd_map.begin(); it != fd_map.end(); it++)
    {
            FD_SET(it->first, &readset);

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
                        HandleRequest(fd);
                }
                else if (FD_ISSET(fd, &writeset))
                {
            std::cout << "send response\n";

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
    FD_SET(new_fd, &readcopy);
    fd_map.insert(std::make_pair(new_fd, 1));
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
        req.fillRequest(buf, rb);
        if (req.isslastRead == true)
        {
            FD_SET(fd, &writecopy);
            FD_CLR(fd, &readcopy);
        }
        // getline(cin, rb);
        // parse request 
        // - first read -> construc5t object
        // - mid reads -> fill body in file
        // - last read -> request is complete clear from read set in write lines below

    }
    else if (rb == 0)
    {
        close(fd);
        FD_CLR(fd ,&readcopy);
    }
}


void Webserver::HandleResponse(int fd)
{
    char *tello = (char *)("HTTP/1.1 200 OK\nContent-length: 17\n\r\nTello from server");
    int returnWrite = write(fd, tello, strlen(tello));
    std::cout << "reurn : " << returnWrite << std::endl;
    std::cout <<  std::strerror( errno) << std::endl;
    FD_CLR(fd, &writecopy);
    close(fd);
}
