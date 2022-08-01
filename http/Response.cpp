#include "Response.hpp"
#include "../Cgi.hpp"

void Response::initData(std::vector<Server>  serv , Request req)
{
    std::map<std::string , std::string > Reqheaders = req.getHeaders();
    int fl = 0;
    if (Reqheaders["Host"] != "")
    {
        for (size_t i = 0 ; i < serv.size() ; i++)
        {
            std::vector<std::string > tmp = split( Reqheaders["Host"], ":");

            if (serv[i].getHost() == tmp[0] &&  fl == 0)
            {
                _server = serv[i];
                fl = 1;
            }
        }
    }
    else
        setStatusCode(400);
    if (fl == 0)
        _server = serv[0];
    _req  = req;
    _path = _req.getUri();
    _method  = _req.getMethod();
    statusCode.first =  req.getStatusCode();
    isEndRes = false;

    validMethod = 0;
    flag = 0;
    file_name = "";
    file_type = "text/html";
    file_size = 0;
    written = 0;
    cgiOn = false;
    findMatchingLocation();
    _loc = matching_location.getLocation();
    _redirect = matching_location.getRedirect();
    _def = matching_location.getDefaultt();
    cgimap = matching_location.getCgiMap();
    setIsvalid();
}

Response::Response(std::vector<Server>  serv , Request req)
{
    // check 400 413 uri errors
    initData(serv, req);
    if (cgimap.size() > 0 )
    {

        Cgi c(_server, req , matching_location); 
        cgiOn = c.getOn();
        if (cgiOn)
        {
            file_name = c.gettoRender_file();
            file_size = fsize( file_name.c_str());  
            cgi_header = c.getHeader();
            return ;
        }
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
   DIR *dr;
   struct dirent *en;
   path =  removeRepeated(path  + "/" + _path, '/');
   std::ofstream bodytmp("body.html", std::ios::out | std::ios::binary);
   dr = opendir(path.c_str());

  bodytmp << "<html>\n<head>\n<body>\n<table>\n";
    if (dr) {
        while ((en = readdir(dr)) != NULL) 
        {
            if(en->d_name != std::string(".") && en->d_name != std::string(".."))
            {
                bodytmp <<  std::string("<br>");
                bodytmp <<  std::string("<a href=\"") + _path + en->d_name + std::string("\">")+ en->d_name + std::string("</a>"); 
            }
        }
   }
   else 
    {
        setStatusCode(404);
        return ;
    }
   bodytmp <<  "</table>\n</body>\n</head>\n</html>\n";
   bodytmp.close();
    file_name ="body.html";
    file_size = fsize(file_name.c_str());   

    setStatusCode(200);

   closedir(dr);
}

void Response::Get()
{
    std::string path = removeRepeated(matching_location.getRoot() +"/", '/');

        if (isDir(path))
        {
            if (_def.size() > 1)
            {
                path = removeRepeated(path +"/" + _loc +"/"+ _def, '/');
                path.erase(path.size() - 1 ) ;
                file_name = path;
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
            if (cgiOn  == true)
                file_name = cgi_file;
            else
                file_name = path;
            size_t index = file_name.find( "?" );
            if (index != std::string::npos)
                file_name.erase(index);
            file_size = fsize(file_name.c_str());
            file_type = get_file_type(file_name);
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

void   Response::findMatchingLocation()
{
    int score = 0;
    int iterations = 0;
    std::map<int , int > locScore;
    locations = _server.getLocations();
    for (size_t i = 0; i < locations.size(); i++)
    {
        score = 0;
        iterations = 0;
        std::string loc = locations[i].getLocation();
        for(size_t i = 0 ; i < _path.size() || i < loc.size() ; i++)
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
    std::string path = _loc + matching_location.getRoot() + "/" +_path;
    path = removeRepeated(path + "/", '/');
    path.erase(path.size() - 1);
    if (access(path.c_str(), F_OK) != -1 )
    {
        if (std::remove(path.c_str()) < 0)
        {
            setStatusCode(403); 
        }
        else
            setStatusCode(200);
        
    }
    else
        setStatusCode(404);

}


void Response::Post()
{

    std::string new_file(_req.getBody().c_str());
    std::string tmp = "./tmp/" + new_file;
    size_t bodyLimit = (_server.getBody_size_limit() * 1024) ;
    size_t filesi = size_t(fsize(tmp.c_str())) ;

    if ((size_t)bodyLimit < filesi)
    {
        setStatusCode(413);
        return ;
    }
    std::string uploadFile  = matching_location.getUpload();
    if (!isDirictory(uploadFile) || uploadFile == "/")
    {
        uploadFile = removeRepeated(matching_location.getRoot() + "/" + uploadFile + "/", '/');
        uploadFile.erase(uploadFile.size());
        mkdir(uploadFile.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    std::string file_name1 = removeRepeated(uploadFile + "/" + new_file, '/');
    std::ifstream in(tmp.c_str(), std::ios::in | std::ios::binary);
    std::ofstream out(file_name1, std::ios::out | std::ios::binary);
    out << in.rdbuf();
    in.close();
    out.close();
    remove(_req.getBody().c_str());
    setStatusCode(201);

}

std::string Response::generateHeader()
{   
    std::string headertmp;

    if (statusCode.first != 200 && statusCode.first != 201 && statusCode.first != 301 && statusCode.first != 302 )
      generateBody();
    
    if (_redirect == "" )
    {  
            headertmp += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
            headertmp += "Content-type: "+ file_type + "\r\n";
            headertmp += "Content-length: " + std::to_string((int)file_size) + "\r\n";
            headertmp += "Server: " + _server.getServerName()[0] + "\r\n";
            //------ to fix cors error
            headertmp += "Access-Control-Allow-Origin: *\r\n";
            headertmp += "Access-Control-Allow-Private-Network: true\r\n";
            headertmp += "Date: " + formatted_time() + "\r\n";
            headertmp += "\r\n";
    }
    else
        generateredeHeader();
    return headertmp;
}


std::pair<char *, size_t> Response::getHeader()
{
    char *buf;   
    header = "";
    if (!cgiOn)
        header +=  generateHeader(); 
    else
        header += cgi_header;

    char * tmp = (char *)malloc(header.length() + 1);
    buf = strcpy(tmp, header.c_str());

    return std::make_pair(buf, strlen(buf));
}
    
std::string Response::generateBody()
{
  std::ofstream bodytmp("body.html", std::ios::out | std::ios::binary);
    if (_server.getErrorpage() == "")
    {
        std::string msg = std::to_string(statusCode.first) +  statusCode.second;
        std::string tmp;
        tmp = "<html>\n<head><title>" + msg + "</title></head>\n<body bgcolor='white'>\n<center><h1>"  +msg + "</h1></center>\n</body>\n</html>";
        bodytmp << tmp;
        file_name = "body.html";
        file_size = tmp.size();
        file_type = "text/html";
        return "body";
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
    else if (code == 302)
        statusCode.second = " found";
    else if (code == 400)
        statusCode.second = " Bad Request";
    else if (code == 413)
        statusCode.second = " Request Entity Too Large";
    else if (code == 500)
        statusCode.second = " Internal Server Error";
}


void Response::generateredeHeader()
{   
        std::string tmp_redirect = matching_location.getRedirect();
    if (tmp_redirect.size())
    {
        std::vector<std::string> v = split(tmp_redirect, " ");
        setStatusCode(std::stoi(v[0]));
        _redirect = v[1];
    }
    header += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
    header += cgi_header;
    header += "Location :" + _redirect + "\r\n";


    header +=  "\r\n";
        
    flag = 1;
}


std::pair<char * , size_t> Response::getBody()
{   
    char *buf;
    int ret;
   
    int fd = open(file_name.c_str(), O_RDONLY);
    std::ofstream fdw("tmp"  );
	buf = (char *)malloc(BUFFER_SIZE);
	lseek(fd, written, SEEK_SET);

	ret = read(fd, buf, BUFFER_SIZE);
    written += ret;

    fdw << buf;
    if ((size_t)written >= file_size)
    {
        fdw.close();
        isEndRes =  true;
    }
	close(fd);
    return std::make_pair(buf, ret);
}