#include "Tools.hpp"

bool isDigit(std::string str)
{
     for (int i = 0 ; i < str.size(); i++)
    {
        int check = isdigit(str[i]);
        if (!check)
            return false;
    }
    return true ;
}