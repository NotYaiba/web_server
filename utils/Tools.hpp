#ifndef  TOOLS_HPP
#define  TOOLS_HPP
 #include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
# define DUP 0
# define EMPTY 1
# define IVA 2
# define SYN 3

# define BUFFER_SIZE 4096



 namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW   = 33,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49    
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };

}
   static  Color::Modifier red(Color::FG_RED);
   static  Color::Modifier green(Color::FG_GREEN);
   static  Color::Modifier yellow(Color::FG_YELLOW);
   static  Color::Modifier blue(Color::FG_BLUE);
   static  Color::Modifier reset(Color::FG_DEFAULT);
class Line;
bool isDigit(std::string str);
std::string &trim(std::string &s);
std::string removeSpaces(std::string str);

std::vector< std::string > split(std::string line, std::string del);
std::string random_string();
std::string  get_file_ext(std::string res);
std::string fixIt(std::string str);
std::string removeRepeated(std::string str , char s);
std::string tuUpper(std::string str);
bool file_exists(std::string filename);
#endif
