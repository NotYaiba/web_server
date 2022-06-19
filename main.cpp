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
        // Socket soc(servers[0]);
        // Socket soc2(servers[1]);
    }
    catch(const char  *er)
    {
        std::cout << er << std::endl;
    }
        

        
    return 0;

}