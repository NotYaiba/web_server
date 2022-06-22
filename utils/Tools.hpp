#ifndef  TOOLS_HPP
#define  TOOLS_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>



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


class Line 
{
    int  _line;
    public:
    int const & getLine() const
    {
        return _line;
    }
    Line()
    {
        _line = 0;
    }
    Line(Line const &lin)
    {
        _line = lin.getLine();
    }
    ~Line()
    {
        
    }
         Line& operator++()       { ++_line; return (*this); }
        Line& operator--()       { --_line; return (*this); }
        
        Line operator++(int)
        {
            Line res(*this);
            ++(*this);
            return (res);
        }
        Line operator--(int)
        {
            Line res(*this);
            --(*this);
            return (res);
        }
};
    static Line lineS;

// class c 
// {
//     public:

// };

// c test;
#endif
