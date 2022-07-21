#include "Response.hpp"
#include "../Cgi.hpp"

void Response::initData(Server  serv , Request req)
{
    _server = serv;
    _req  = req;
    _path = _req.getUri();
    _method  = _req.getMethod();
    statusCode.first =  req.getStatusCode();
    isEndRes = true;
    /////

    validMethod = 0;
    flag = 0;
    file_name = "";
    file_type = "text/html";
    file_size = 0;
    written = 0;
    cgimap = _server.getCgiMap();
    findMatchingLocation();
    _loc = matching_location.getLocation();
    _redirect = matching_location.getRedirect();
    _def = matching_location.getDefaultt();
    setIsvalid();
}
Response::Response(Server  serv , Request req)
{

    // check 400 413 uri errors
    initData(serv, req);
    if (serv.getCgiMap().size() > 0)
    {
        std::cout << yellow <<matching_location.getRoot() <<reset << std::endl;
        Cgi c(serv, req , matching_location); 
    }
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
        setStatusCode(405);
    // if get method | 404 403 405 | 200 | 301 redirect

    // if delete Method | 405 404 403 | 200

    // if post method | 405 | 201

    // addistional info in CGI case 500 is returnrd when CGI fails
    // else 500
}

bool Response::isDir(std::string path)
{
    std::string tmp =  removeRepeated(path + '/'+ _path , '/');
    struct stat statbuf;
	if (tmp == "/")
		return true;
	std::string s = tmp;
	if (stat(s.c_str(), &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}



void Response::getIndex(std::string path)
{
    // std::cout << "get index \n";
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
   else 
    {
        setStatusCode(404);
        return ;
    }
   this->body += "</table>\n</body>\n</head>\n</html>\n";
//    std::cout <<" end index \n";
   flag = 1;
   file_size = body.size();
    setStatusCode(200);

   closedir(dr);
}

void Response::Get()
{
    std::string path = removeRepeated(matching_location.getRoot() +"/", '/');
    // std::cout << "path: " <<  path << std::endl;
    if (_redirect != "")
    {
        setStatusCode(301);
        generateredeHeader();
        path = _redirect;
        // std::cout << "yoooo: " << _redirect.substr(3, _redirect.length()) << std::endl;
    }
    else
    {
        if (isDir(path))
        {
            std::cout  << "is Dir " <<_def.size()  << std::endl;
            if (_def.size() > 1)
            {
                path = removeRepeated(path +"/" + _loc +"/"+ _def, '/');
                path.erase(path.size() - 1 ) ;
                file_name = path;
                std::cout << "def on " <<  file_name << std::endl;
                if (access(file_name.c_str(), R_OK) == -1 && access(file_name.c_str(), F_OK) == 0)
                {
                    setStatusCode(403);
                    return ;
                }
                else if (access(file_name.c_str(), F_OK) == -1)
                {
                    file_size = 0;

                    setStatusCode(404);
                    return ;
                }
                file_size = fsize(file_name.c_str());
                file_type = get_file_type(file_name);
            }
            else
            {
                if (matching_location.getAutoindex() == false)
                    setStatusCode(403);
                else
                     getIndex(path);
                return ;
                
            }
        }
        else
        {
            path =  removeRepeated(path + '/'+ _path , '/');
            path.erase(path.size() - 1);
            if (cgimap.size() > 0)
                file_name = "index.html";
            else
                file_name = path;
            file_size = fsize(file_name.c_str());
            file_type = get_file_type(file_name);

            std::cout << "file name " << file_name << std::endl;
            std::cout << "file size " << file_size << std::endl;
            std::cout << "file type " << file_type << std::endl;
            if (access(file_name.c_str(), R_OK) == -1 && access(file_name.c_str(), F_OK) == 0)
            {
                setStatusCode(403);
                return ;
            }
            else if (access(file_name.c_str(), F_OK) == -1)
            {
                file_size = 0;
                setStatusCode(404);
                return ;
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

void Response::setIsvalid()
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
    // std::cout << "file to delete " << _path << std::endl;
    std::string path = _loc + matching_location.getRoot() + "/" +_path;
    // std::cout << "where : " << path<< std::endl;
    path = removeRepeated(path, '/');
    path.erase(path.size() - 1);
    // std::cout << "removed : " <<path<< std::endl;
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
    // std::cout << blue << new_file << reset<<std::endl;
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
    if  (!flag && file_size == 0)
    {
        if (statusCode.first != 200 && statusCode.first !=  201)
            body = generateBody();  
    }
    if (_redirect == "" )
    {  
            header += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
            header += "Content-type: "+ file_type + "\r\n";
            header += "Content-length: " + std::to_string((int)file_size) + "\r\n";
            header += "Server: mywebserver\r\n";
            header += "Date: " + formatted_time() + "\r\n";
            header += "\r\n";
            if (flag ==  1)
            {
                header += body;
                // std::cout << header;
                // std::cout << "END HEADERS\n";
            }
            else
            {
                // std::cout << "salam\n";
                isEndRes = false;
            }
    }
}

std::pair<char *, size_t> Response::getHeader()
{
     char *buf;   
    generateHeader(); 
	buf = (char *)malloc(sizeof(char) * header.size());
    buf = strcpy(new char[header.length() + 1], header.c_str());
    // std::cout << "Please10 : " << getIsend() << std::endl;

    return std::make_pair(buf, strlen(buf));
}

std::string Response::generateBody()
{

    if (_server.getErrorpage() == "")
    {
    std::string msg = std::to_string(statusCode.first) +  statusCode.second;
    std::string tmp;
    tmp = "<html>\n<head><title>" + msg + "</title></head>\n<body bgcolor='white'>\n<center><h1>"  +msg + "</h1></center>\n</body>\n</html>";
        flag = 1;
        file_size = tmp.size();
        file_type = "text/html";
        return tmp;
    }
    else
    {
        file_name = (_server.getErrorpage() + "/" + std::to_string(statusCode.first) + ".html");
        file_size = fsize(file_name.c_str());
        file_type = "text/html";
    }
    return ("");
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
        statusCode.second = " Not Allowed";
    else if (code == 301)
        statusCode.second = " Moved Permanently";
}


void Response::generateredeHeader()
{   
    header += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
    header += "Location :" + _redirect.erase(0, 3) + "\r\n";
    header +=  "\r\n";
    flag = 1;
}

std::pair<char * , size_t> Response::getBody()
{   
    char *buf;
    int ret;
    int len;
    std::cout << red << "File to render " << file_name << reset<< std::endl;
    int fd = open (file_name.c_str(), O_RDONLY);
	buf = (char *)malloc(BUFFER_SIZE);
	lseek(fd, written, SEEK_SET);
	if (fd == -1)
    {
		perror("open");
    }
	ret = read(fd, buf, BUFFER_SIZE);
    written += ret;
	if (ret == -1)
		perror("read");
    // std::cout << ret << std::endl;
    // std::cout << buf << std::endl;
    // std::cout << file_size << std::endl;
    // std::cout << written << std::endl;
    if (written >= file_size)
        isEndRes =  true;
	close(fd);
    return std::make_pair(buf, ret);
}