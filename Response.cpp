#include "Response.hpp"

Response::Response(Server  serv , Request req)
{
    std::cout << red << "\n\n start Response \n\n" << reset <<  std::endl;
    _server = serv;
    _req  = req;
    _path = _req.getUri();
    std::cout << "_path: " <<  _path << std::endl;
    _method  = _req.getMethod();
    validMethod = 0;
    flag = 0;
    statusCode.first =  req.getStatusCode();
    // if (statusCode != 200 || statusCode != 201)
    // {
    //          // todo send response with status code; 
    // }
    // check 400 413 uri errors
    if (statusCode.first == 400 || statusCode.first == 413)
    {

    }
    findMatchingLocation();
    _loc = matching_location.getLocation();
    std::cout << "location: " <<  _loc << std::endl;
    _redirect = matching_location.getRedirect();
    _def = matching_location.getDefaultt();
    KaynatMethod();
    // matching_location.debug();
    if (validMethod)
    {
        if (_method == "DELETE")
            Delete();
        else if (_method == "GET")
            Get();
        else if (_method == "POST")
            Post();

    }
    else
        std::cout << "meeh\n";
    generateHeader();
    // if get method | 404 403 405 | 200 | 301 redirect

    // if delete Method | 405 404 403 | 200

    // if post method | 405 | 201


    // addistional info in CGI case 500 is returnrd when CGI fails
    // else 500
}

bool Response::isDir(std::string path)
{
    std::string tmp =  removeRepeated(path + '/'+ _path , '/');
    std::cout << blue << tmp << std::endl;
    std::cout << blue << tmp.erase(tmp.size() - 1) << std::endl;
    struct stat statbuf;
	if (tmp == "/")
		return true;
	std::string s = tmp;
	if (stat(s.c_str(), &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}

void Response::generateredeHeader()
{   
    int len;
    body = generateBody();
    len = body.size();
    header += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
    header += "Location :" + _redirect.erase(0, 3) + "\r\n";
    header +=  "\r\n";

}

void Response::getIndex(std::string path)
{
    std::cout << "get index \n";
   DIR *dr;
   struct dirent *en;
   path =  removeRepeated(path  + "/" + _path, '/');
   std::cout << "path  :" << path << std::endl;
   dr = opendir(path.c_str());

   this->body = "<html>\n<head>\n<body>\n<table>\n";
    if (dr) {
        while ((en = readdir(dr)) != NULL) 
        {
            if(en->d_name != std::string(".") && en->d_name != std::string(".."))
            {
                this->body += std::string("<br>");
                this->body += std::string("<a href=\"") + _path + en->d_name + std::string("\">")+ en->d_name + std::string("</a>"); 
            }
        }
   }
   this->body += "</table>\n</body>\n</head>\n</html>\n";
   std::cout <<" end index \n";
   flag = 1;
   closedir(dr);
}

void Response::Get()
{
    std::string path = removeRepeated(matching_location.getRoot() +"/", '/');
    std::cout << "path: " <<  path << std::endl;
    if (_redirect != "")
    {
        setStatusCode(301);
        path = _redirect;
        // std::cout << "yoooo: " << _redirect.substr(3, _redirect.length()) << std::endl;
    }
    else
    {
        if (isDir(path ))
        {
            std::cout  << _def.size()  << std::endl;
            if (_def.size() > 1)
            {
                path = removeRepeated(path +"/" + _def, '/');
                path.erase(path.size() - 1 ) ;
                std::ifstream in(path.c_str(), std::ios::in | std::ios::binary);
                std::stringstream strStream;
                strStream << in.rdbuf(); 
                body = strStream.str();
                flag = 1;
                in.close();
            }
            else
            {
                if (matching_location.getAutoindex() == false)
                {
                    setStatusCode(403);
                    return ;
                }
                else
                     getIndex(path);
            }
        }
        else
        {
            path =  removeRepeated(path + '/'+ _path , '/');
            path.erase(path.size() - 1);
            if (access(path.c_str(), R_OK) == -1 && access(path.c_str(), F_OK) == 0)
            {
                setStatusCode(403);
                return ;
            }
            else if (access(path.c_str(), F_OK) == -1)
            {
                std::cout << "yes you are right\n";
                setStatusCode(404);
                return ;
            }
            else
            {
                std::ifstream f(path);
                std::string str;
                if(f) {
                    std::ostringstream ss;
                    ss << f.rdbuf();
                    ss << std::endl;
                    str = ss.str();
                }
                body = str;
                flag = 1;
            }
        }
        setStatusCode(200);
    }
}

void   Response::findMatchingLocation()
{
    int score = 0;
    int iterations = 0;
    std::map<int , int > locScore;
    locations = _server.getLocations();
    for (int i = 0; i < locations.size(); i++)
    {
        score = 0;
        iterations = 0;
        std::string loc = locations[i].getLocation();
        for(int i = 0 ; i < _path.size() || i < loc.size() ; i++)
        {
                if (_path[i] == loc[i])
                {
                    if (loc[i] == '/')
                        score++;
                    iterations++;
                }
                else
                { 
                    iterations = loc.size();
                    break;
                }
        }
        std::pair<std::map<int ,int >::iterator , bool > ret = locScore.insert(std::make_pair(score, iterations));
        if (ret.second == false)
        {
            if (iterations < (ret.first)->second)
            {
                if (ret.first->first >= (--locScore.end())->first)
                {
                    locScore[score] = iterations;
                    matching_location = locations[i];
                }
            }
        }
        else if (score >= (--locScore.end())->first )
            matching_location = locations[i];
    }
}

void Response::KaynatMethod()
{
    std::string tmp = matching_location.getMethod();
    tmp = trim(tmp);
    std::vector <std::string> v = split(tmp, " ");
    for (size_t i = 0 ; i < v.size() ; i++)
    {
        if (_method == v[i] )
        {
            validMethod = 1;  
            return ;
        }
    }
    validMethod = 0;
    
}



void Response::Delete()
{
    matching_location.debug();
    std::cout << "file to delete " << _path << std::endl;
    std::string path = _loc + matching_location.getRoot() + "/" +_path;
    std::cout << "where : " << path<< std::endl;
    path = removeRepeated(path, '/');
    path.erase(path.size() - 1);
    std::cout << "removed : " <<path<< std::endl;
    if (file_exists(path))
    {
        if (std::remove(path.c_str()) < 0)
            setStatusCode(403);
        else
            setStatusCode(200);
    }
    else
        setStatusCode(404);

}


void Response::Post()
{
    //  if (s.get_max_body_size() < size_t(fsize(req.get_body().c_str())))
    // {
    //     std::cout << "Request body too large" << std::endl;
    //     remove(req.get_body().c_str());
    //     set_status_code(413);
    //     return ;
    // }
    std::string new_file(_req.getBody().c_str());
    std::cout << blue << new_file << reset<<std::endl;
    std::string file_name = removeRepeated(matching_location.getUpload() + "/" + new_file, '/');
    std::string tmp = "./tmp/" + new_file;
    std::ifstream in(tmp.c_str(), std::ios::in | std::ios::binary);
    std::ofstream out(file_name, std::ios::out | std::ios::binary);
    out << in.rdbuf();
    in.close();
    out.close();
    remove(_req.getBody().c_str());
    setStatusCode(201);
}

void Response::generateHeader()
{   
    int len;
    if  (!flag)
        body = generateBody();
    len = body.size();
    header += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
    header += "Content-type:  text/html\r\n";
    header += "Content-length: " + std::to_string(len) + "\r\n";
	header += "Server: mywebserver\r\n";
    header += "Date: " + formatted_time() + "\r\n";
    header += "\r\n"+ body;
}

std::string Response::gethadak()
{
    return header;
}

std::string Response::generateBody()
{

   std::string msg = std::to_string(statusCode.first) +  statusCode.second;
   std::string tmp;
   tmp = "<html>\n<head><title>" + msg + "</title></head>\n<body bgcolor='white'>\n<center><h1>"  +msg + "</h1></center>\n</body>\n</html>";
   return tmp;
}

void Response::setStatusCode(int code)
{
    statusCode.first = code;
    if (code == 200)
        statusCode.second = " OK";
    else if (code == 404)
        statusCode.second = " Not Found";
    else if (code == 403)
        statusCode.second = " Forbidden";
    else if (code == 201)
        statusCode.second = " Created";
    else if (code == 405)
        statusCode.second = " leeda";
    else if (code == 301)
        statusCode.second = " Moved Permanently";
}
