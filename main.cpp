#include "Parser.hpp"
#include "Socket.hpp"
#include "./utils/Tools.hpp"


int main(int ac , char *av[])
{
    (void)ac;
    (void)av;

    try{
        Parser pars(ac, av);
        std::vector < Server > servers = pars.getServers();
        servers[0].debug();
        Socket soc(servers[0]);
            sockaddr_in client;
        socklen_t clientSize = sizeof(client);
    char buf[4096];
        while (true)
        {
            fcntl(soc.getFd(), F_SETFL,  O_NONBLOCK);
            std::cout << soc.getFd() << std::endl;
            int new_socket = accept(soc.getFd(), (struct sockaddr *)&client, &clientSize);
            // std::cout << "Received call..." << std::endl;
            // if (new_socket == -1)
            // {
            //     std::cerr << "Problem with client connecting!";
            //     return -4;
            // }
            // memset(buf, 0, 4096);

            // int bytesRecv = recv(new_socket, buf, 4096, 0);
            // if (bytesRecv == -1)
            // {
            //     std::cerr << "There was a connection issue." << std::endl;
            // }
            // if (bytesRecv == 0)
            // {
            //     std::cout << "The client disconnected" << std::endl;
            // }
            // std::cout << "Received:2 " << buf << std::endl;
            // char *tello = (char *)("HTTP/1.1 200 OK\nContent-length: 17\n\r\nTello from server");
            // write(new_socket, tello, strlen(tello));
        }
    }
    catch(const char  *er)
    {
        std::cout << er << std::endl;
    }
        

        
    return 0;

}