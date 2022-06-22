#include "Parser.hpp"
#include "Socket.hpp"
#include "./utils/Tools.hpp"
#include <sys/time.h>

#include <sys/types.h>

#include <sys/select.h>
#include <cerrno>
# define BUFFER_SIZE 4096

int main(int ac , char *av[])
{
    (void)ac;
    (void)av;

    try{
        Parser pars(ac, av);
        std::vector < Server > servers = pars.getServers();
        pars.debug();
        // for (size_t i  = 0 ; i < servers.size(); i++)
        //     servers[i].debug();
        // Socket soc(servers[0].getPort());
        
        // char buf[BUFFER_SIZE];
        // int socket = soc.getFd();
        // fd_set readset;
        // fd_set writeset;

        // FD_ZERO(&readset); 
        // FD_ZERO(&writeset); 
        // FD_SET(socket, &readset);
        // fd_set readcopy;
        // fd_set writecopy;

        // FD_ZERO(&readcopy); 
        // FD_ZERO(&writecopy); 
        // readcopy = readset;
        // int maxfd = socket;
        // for (;;)
        // {
        //     readset = readcopy;
        //     writeset = writecopy;
        //     // NULL timeout (5-th argument) means wait until event
        //     int rs = select(maxfd + 1, &readset, &writeset, NULL, NULL);
        //     if (rs == -1)
        //         throw "errooor";
        //     std::cout <<"rs   : " << rs << std::endl;
        //     for (int fd = 0; fd <= maxfd; fd++)
        //     {
        //         if (FD_ISSET(fd, &readset) || FD_ISSET(fd, &writeset))
        //         {
        //             if (FD_ISSET(fd, &readset))
        //             {
        //                 if (fd == socket)
        //                 {
        //                     int new_fd = soc.AccectSock(fd);
        //                     fcntl(new_fd, F_SETFL,  O_NONBLOCK);
        //                     FD_SET(new_fd, &readcopy);
        //                     if (new_fd > maxfd)
        //                        maxfd = new_fd;
        //                 }
        //                 else 
        //                 {
        //                     int rb = read(fd,buf,BUFFER_SIZE );
        //                     if (rb > 0)
        //                     {
        //                         std::cout << buf << std::endl;
        //                         // parse request 
        //                         // - first read -> construc5t object
        //                         // - mid reads -> fill body in file
        //                         // - last read -> request is complete clear from read set in write lines below
        //                         FD_SET(fd, &writecopy);
        //                         FD_CLR(fd, &readcopy);

        //                     }
        //                     else if (rb == 0)
        //                     {
        //                         close(fd);
        //                         FD_CLR(fd ,&readcopy);
        //                     }
        //                 }
        //             }
        //             else if (FD_ISSET(fd, &writeset))
        //             {
        //                 char *tello = (char *)("HTTP/1.1 200 OK\nContent-length: 17\n\r\nTello from server");
        //                 write(fd, tello, strlen(tello));
        //                 FD_CLR(fd, &writecopy);
        //                 close(fd);
        //             }
        //         }
        //     }
        // } 
    }
    catch(const char  *er)
    {
        std::cout << er << std::endl;
    }
        

        
    return 0;

}