#include "Parser.hpp"
#include "Socket.hpp"
#include "Connection.hpp"
#include "./utils/Tools.hpp"
#include <sys/time.h>
#include  "WebServer.hpp"
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
        Connection connection(pars.getMapServers());
        Webserver ws(connection);
        ws.RunWebServer();
    }
    catch(std::string  er)
    {
        std::cout << er << std::endl;
    }
        

        
    return 0;

}