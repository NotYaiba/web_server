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
        line.erase(start, found + del.size()) ;
        ret = trim(ret);


        if (ret != "")
            v.push_back(ret);
        
        found = line.find(del ,0);

    }
    
        v.push_back(line);
    return (v);
}

std::string     get_file_ext(std::string res)
{
    if (!res.empty())
	{
		if ("text/html" == res) return(".html");
		else if ("text/css" == res) return(".css");
		else if ("text/xml" == res) return(".xml");
		else if ("text/csv" == res) return(".csv");
		else if ("image/gif" == res) return(".gif");
		else if ("image/x-icon" == res) return(".ico");
		else if ("image/jpeg" == res) return(".jpeg");
		else if ("application/javascript" == res) return(".js");
		else if ("application/json" == res) return(".json");
		else if ("image/png" == res) return(".png");
		else if ("application/pdf" == res) return(".pdf");
		else if ("image/svg+xml" == res) return(".svg");
		else if ("text/plain" == res) return(".txt");
		else if ("application/atom+xml" == res) return(".atom");
		else if ("application/rss+xml" == res) return(".rss");
		else if ("image/webp" == res) return(".webp");
		else if ("video/3gpp" == res) return(".3gpp");
		else if ("video/3gpp" == res) return(".3gp");
		else if ("video/mp2t" == res) return(".ts");
		else if ("video/mp4" == res) return(".mp4");
		else if ("video/mpeg" == res) return(".mpeg");
		else if ("video/mpeg" == res) return(".mpg");
		else if ("video/quicktime" == res) return(".mov");
		else if ("video/webm" == res) return(".webm");
		else if ("video/x-flv" == res) return(".flv");
		else if ("video/x-m4v" == res) return(".m4v");
		else if ("video/x-mng" == res) return(".mng");
		else if ("video/x-ms-asf" == res) return(".asx");
		else if ("video/x-ms-asf" == res) return(".asf");
		else if ("application/javascript" == res) return(".js");
		else if ("video/x-ms-wmv" == res) return(".wmv");
		else if ("video/x-msvideo" == res) return(".avi");
		else if ("audio/midi" == res) return(".mid");
		else if ("audio/midi" == res) return(".midi");
		else if ("audio/midi" == res) return(".kar");
		else if ("audio/mpeg" == res) return(".mp3");
		else if ("audio/ogg" == res) return(".ogg");
		else if ("audio/x-m4a" == res) return(".m4a");
		else if ("audio/x-realaudio" == res) return(".ra");
		else if ("application/x-perl" == res) return(".pl");
		else if ("application/x-python" == res) return(".py");
		else if ("application/x-php" == res) return(".php");
		else if ("application/x-c++" == res) return(".cpp");
		else if ("application/x-c" == res) return(".c");
		else if ("application/zip" == res) return(".zip");
	}
    return "";
}

#include <random>
#include <string>

std::string random_string()
{
     std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

     std::random_device rd;
     std::mt19937 generator(rd());

     std::shuffle(str.begin(), str.end(), generator);

     return str.substr(0, 32);    // assumes 32 < number of characters in str         
}
std::string fixIt(std::string str)
{
    str += "/";
    std::cout <<str <<std::endl;
    return (removeRepeated(str, '/'));
    
}
std::string removeRepeated(std::string str , char s)
{
    std::vector <char> v;
    int d = 0;
    for (int i = 0 ; i < str.size(); i++)
    {
        
        if (s == str[i] )
        {
            if (d == 0)
            {
                v.push_back(str[i]);
                d = 1;

            }
           continue ;
        }
        else
            d = 0;
        v.push_back(str[i]);
    }
    int size  = v.size();
    char ss[1000];
    
    int k = 0;
    for (k= 0 ; k < size; k++)
        ss[k] = v[k];
    ss[k] = '\0';
    return ss;
}

std::string tuUpper(std::string str)
{
    std::string ret; 
    for (size_t i = 0; i < str.size(); i++)
    {
        /* code */
        ret.append(1, str[i]);
    }
    return (ret);
}

bool file_exists(std::string filename)
{
	int fd = open(filename.c_str(), O_RDONLY);
	std::cout << "fd: >>>>" << fd << std::endl;
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

std::string	formatted_time()
{
	time_t	current;
	struct tm * timeinfo;
	char	buffer[80];

	time(&current);
	timeinfo = localtime (&current);

	strftime(buffer, 80, "%a,%e %b %Y %X %Z", timeinfo);

	return std::string(buffer);
}