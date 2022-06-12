#include "Parser.hpp"


int main(int ac , char *av[])
{
    (void)ac;
    (void)av;
    try{
        Parser pars(ac, av);
    }
    catch(const char  *er)
    {
        std::cout << er << std::endl;
    }
        

        
    return 0;

}