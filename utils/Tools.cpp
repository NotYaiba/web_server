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
// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

std::string removeSpaces(std::string str)
{
str.erase(remove(str.begin(), str.end(), ' '), str.end());
return str;
}


std::vector< std::string > split(std::string line, std::string del)
{
    std::string ret = "";
    std::vector< std::string > v;
    std::size_t found;
    int start = 0;
    found = line.find(del, 0);

    while (found != std::string::npos)
    {
        ret = line.substr(start,  found  - start);
        line.erase(start, found +  1);
        ret =trim(ret);
        if (ret != "")
            v.push_back(ret);
        found = line.find(del , found + 1);
    }
    v.push_back(line);
    return (v);
}