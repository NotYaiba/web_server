#include "Location.hpp"

    Location::Location(): location("") , method(""), root("") , defaultt("") , upload("") , redirect("")
    {

    }
    Location::~Location()
    {

    }

    
    void Location::setLocation(std::string const &rot)
    {

        std::string tmp =  rot;
        if (tmp == "")
            throwError(EMPTY, "location");
        if(tmp[tmp.size() - 1] == '[')
            tmp.erase(tmp.size() - 1);
        else
            throwError(SYN, "location");
        if (location == "")
            location = fixIt(tmp);
        else
           throwError(DUP , "location");
    }
    std::string const & Location::getLocation()const 
    {
        return location;
    }
    void Location::setMethod(std::string const &rot)
    {
        if (rot == "")
            throwError(EMPTY, "method");
        if (method == "")
            method = rot;
        else
            throwError(DUP , "method");

    }
    std::string const & Location::getMethod()const 
    {
        return method;
    }

    void Location::setAutoindex(std::string const &rot)
    {
        if (rot == "")
            throwError(EMPTY, "autoindex");
        if (rot == "on")    
            autoindex = true;
        else if (rot == "off")    
            autoindex = false;
        else
           throwError(IVA, "autoindex");

    }
    bool Location::getAutoindex()const 
    {
        return autoindex;
    }

    void Location::setDefaultt(std::string const &rot)
    {
        if (rot == "")
            throwError(EMPTY, "default");
        if (defaultt == "")
            defaultt = fixIt(rot);
        else
            throwError(DUP , "default"); 
    }
    std::string const & Location::getDefaultt()const 
    {
        return defaultt;
    }

    void Location::setUpload(std::string const &rot)
    {
         if (rot == "")
            throwError(EMPTY, "upload");
        if (upload == "")
            upload = rot;
        else
            throwError(DUP , "upload");  
    }
    std::string const & Location::getUpload()const 
    {
        return upload;
    }
    void Location::setRoot(std::string const &rot)
    {


        if (rot == "")
            throwError(EMPTY, "root");
        if (root == "")
            root = rot;
        else
            throwError(DUP, "root");
        
    }
    void Location::setRedirect(std::string const &rot)
    {
        if (redirect == "")
            redirect = rot;
        else
            throwError(DUP, "redirect");
    }
    std::string const & Location::getRedirect()const 
    {
        return redirect;
    }
    std::string const & Location::getRoot()const 
    {
        return root;
    }


void    Location::debug()
{
    std::cout <<"--------------------------------------------------------" << std::endl; 
    std::cout << std::setw(5) << red << "location :" << reset  << location << std::endl; 
    std::cout << std::setw(5) << red << "method :" << reset  << method << std::endl; 
    std::cout << std::setw(5) << red << "root :" << reset  << root<< std::endl; 
    std::cout << std::setw(5) << red << "autoindex :" << reset  << autoindex << std::endl; 
    std::cout << std::setw(5) << red << "defaultt :" << reset  << defaultt << std::endl; 
    std::cout << std::setw(5) << red << "upload :" << reset  << upload << std::endl; 
    std::cout <<"--------------------------------------------------------" << std::endl; 
}

void Location::throwError(int type, std::string para)
{
    std::string nb = std::to_string(line);
    nb = "Line : " + nb; 
    std::stringstream errr(nb);
    switch (type)
    {
        case DUP :
            errr << red <<nb <<  reset <<  "  ERROR Duplicated Parameter " << para   ;
            break;
        case EMPTY :
            errr << red <<nb <<  reset <<  "  ERROR Empty Parameter " << para   ;
        case IVA :
            errr << red <<nb <<  reset <<  "  ERROR Invalid Parameter " << para   ;
            break;
        
    }
        throw errr.str();

}

Location &Location::operator=(Location  const & src)
{
    location = src.getLocation();
    method = src.getMethod();
    root = src.getRoot();
    autoindex  =src.getAutoindex();
    defaultt = src.getDefaultt();
    upload = src.getUpload();
    redirect = src.getRedirect();
    cgimap = src.getCgiMap();
    return (*this);
}
    Location::Location(Location  const & src)
    {
        *this = src;

        // std::cout << yellow << "ya salam \n";
    }

    void Location::addtoCgiMap(std::string key, std::string val)
    {
    cgimap.insert(std::make_pair(key, val));
    }

    std::map<std::string, std::string> const &  Location::getCgiMap() const 
    {
    return cgimap;
    }