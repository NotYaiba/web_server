#include "Cgi.hpp"

Cgi::Cgi(Server serv , Request  req , Location const & loc) : _server(serv), _req(req), _loc(loc)
{
    toRender_file= ".tmp/index-" + random_string();
    _status = "";
    on = false;
    is_post = false;
    initData();
    if (on == true)
    {
        execute_cgi();
        pars_file();
    }
}
void Cgi::initData()
{
    cgimap = _loc.getCgiMap();
    _uri =_req.getUri();
    _uri.erase(_uri.size() - 1);
    _path = _uri;
    size_t found = _path.find("?");
    if (found != std::string::npos)
        _path.erase(found);
    filepath =  removeRepeated(_loc.getRoot() + "/" +  _uri  , '/');
    std::vector<std::string> tmpv = split(filepath, "?");
    if(tmpv.size() > 1)
    {
        _query = tmpv[1];
        filepath = tmpv[0];
        filepath = removeRepeated(filepath + "/" , '/');
        filepath.erase(filepath.size() - 1);
    }
    std::string def =  _loc.getDefaultt();
    if (def.size() > 1 && isDirictory(filepath))
    {
        filepath = removeRepeated(filepath +"/" + def + "/", '/');
        _path = removeRepeated(_path +"/" + def + "/", '/');
        filepath.erase(filepath.size() - 1);
    }
    _method = _req.getMethod();
    filetype =  get_file_type(filepath);

    if (_method == "POST")
        is_post = true;
    if (isDirictory(filepath) ||( filetype != "application/x-php" && filetype != "application/x-python" ))
        return ;
    else
    {
        if (filetype == "application/x-php")
            _cgikey = cgimap["php"];
        else
            _cgikey = cgimap["python"];
        if (_cgikey == "")
            return ;
        on = true;
    }
}

void Cgi::execute_cgi()
{
    pid_t pid;
    int post_fd;
    int outfile_fd = open("./index.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    post_fd = open(("tmp/" + _req.getBody()).c_str(),   O_RDONLY , 0666);
    arr = initarr();
    SetEnv();
    pid = fork();

    if (pid == 0)
    {
        if (is_post)
            dup2(post_fd, STDIN_FILENO);
        dup2(outfile_fd, STDOUT_FILENO);
        close(outfile_fd);
        if (is_post)
            close(post_fd);
        if (execve(arr[0] ,arr, env) < 0)
        {
            std::cout << "Status: 500 Internal Server Error";
            exit(0);
        }

    }
    close(outfile_fd);
    if (is_post)
        close(post_fd);
    while (wait(NULL) > 0);
    for (int i = 0; arr[i] != nullptr; i++) 
        free(arr[i]);
    free(arr);
}
void Cgi::SetEnv()
{
    std::string def = removeRepeated(_loc.getDefaultt() + "/" , '/');
    def.erase(def.size() - 1);
    std::string scriptUri = removeRepeated(_req.getUri() , '/');
    std::string scriptPath = removeRepeated(_loc.getRoot() + "/" + scriptUri , '/');
    if (isDirictory(scriptPath))
    {
        scriptUri = removeRepeated(scriptUri +"/"+  def  , '/');
        scriptPath = removeRepeated(scriptPath + "/" + def , '/');
    }
    std::map<std::string , std::string> mp;
    std::map<std::string , std::string> req_headers = _req.getHeaders();
    for(std::map<std::string , std::string>::iterator it = req_headers.begin() ; it != req_headers.end() ; it++ )
    {
        std::string key("HTTP_");
        key += it->first;
        std::transform(key.begin(), key.end(), key.begin(), asciiToUpper);
        mp[key] = it->second;
    }
    std::vector<std::string> remote = split(req_headers["Host"], ":");
    mp["QUERY_STRING"] = _query;
    mp["REQUEST_METHOD"] = _method;
    mp["CONTENT_TYPE"] = _req.getContentType();
    mp["CONTENT_LENGTH"] = std::to_string(_req.getContentLength());
    mp["SCRIPT_NAME"] = removeRepeated( _uri+ "/" + def, '/') ;
    if (isDirictory(_req.getUri()))
        mp["REQUEST_URI"] = removeRepeated( _uri+ "/" + def, '/');
    else
        mp["REQUEST_URI"] = removeRepeated(  _uri+ "/" , '/');
    mp["DOCUMENT_URI"] = scriptPath;
    mp["DOCUMENT_ROOT"] = _loc.getRoot();
    mp["SERVER_PROTOCOL"] = "HTTP/1.1";
    mp["REQUEST_SCHEME"] = "http";
    mp["GATEWAY_INTERFACE"] = "CGI/1.1";
    mp["SERVER_SOFTWARE"] = "webserv/2.1";
    mp["PATH_TRANSLATED"] = filepath;
    mp["REMOTE_ADDR"] = remote[0];
    mp["REMOTE_PORT"] = remote[1];
    mp["SERVER_ADDR"] = _server.getHost();
    mp["SERVER_PORT"] = std::to_string(_server.getPort());
    mp["SERVER_NAME"] = (_server.getServerName())[0];
    mp["REDIRECT_STATUS"] = "200";
    mp["SCRIPT_FILENAME"] =  removeRepeated(_loc.getRoot() + "/" + _path, '/');
    mp["HTTP_COOKIE"] = _req.getCookie();
    std::vector<std::string> v;
    for (std::map<std::string, std::string>::iterator it = mp.begin(); it != mp.end(); it++)
    {
        std::string tmp = it->first + "=" + it->second;
        v.push_back(tmp);
    }
	env = vectToArr(v);
}
char **  Cgi::initarr()
{
    std::vector<std::string> ar;
	ar.push_back(_cgikey);
	ar.push_back(filepath);
    return(vectToArr(ar));
}

std::string Cgi::find(std::string str, std::string line)
{
    size_t found = line.find(str);
    if (found != std::string::npos)
        return line;
    return "";
}

void Cgi::pars_file()
{
    std::ifstream MyReadFile("index.html");
    if (_status == "301")
    {
         _header += "HTTP/1.1 "+ _status + _header + "\r\n";
      _header  += _location + "\r\n";
      _header  +=  "\r\n";
        return ;

    }
    _header += "Server: mywebserver\r\n";

    _header += "Access-Control-Allow-Origin: *\r\n";
    _header += "Access-Control-Allow-Private-Network: true\r\n";
    for (std::string line; std::getline(MyReadFile, line);) 
    {
        if (line == "\r")
            break;
        std::string tmp;
        if ((tmp = find("Location", line)) != "")
            _location = tmp;
        if ((tmp = find("Status", line)) != "")
        {
            _status = tmp;
            continue;
        }
        if ((tmp = find("Content-length", line)) != "")
            _content_length = true;
        _header += line + "\r\n";
    }
    int newfile_fd = open(toRender_file.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(newfile_fd, STDOUT_FILENO);
    for (std::string line; std::getline(MyReadFile, line);) 
    {
        std::cout << line;
    }
    size_t file_size = fsize(toRender_file.c_str());
    std::cout << std::endl;
    if (_content_length == false)
    {
        _header += "Content-length: "  + std::to_string(file_size) + "\r\n";
    }

    if (_status.size())
    {
        std::vector<std::string> v = split(_status, ":");
         _status = v[1];
        v = split(v[1], " ");
        statusCode.first = v[0] ;
        statusCode.second = v[1] ;
        _header = "HTTP/1.1 "+ _status + _header + "\r\n";
    }
    else
        _header = "HTTP/1.1 200 OK\n\r"  + _header + "\r\n";
    if (_location.size())
    {
        std::vector<std::string> v = split(_location, ": ");
        _location = v[1];
    }

    if (statusCode.first != "200" && statusCode.first != "201" && statusCode.first != "302" &&  statusCode.first != "" && !file_size)
        generateBody();
    close(newfile_fd);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}

int Cgi::getStatus() const 
{
    std::string s = statusCode.first;
    if (s != "")
        return std::stoi(statusCode.first);
    return 200;
  
}

std::string Cgi::generateBody()
{
    if (access(std::string(_server.getErrorpage() + "/" + statusCode.first + ".html" ).c_str(), F_OK) == -1)
    {
        std::string msg = _status;
        std::string tmp;
        tmp = "<html>\n<head><title>" + msg + "</title></head>\n<body bgcolor='white'>\n<center><h1>"  +msg + "</h1></center>\n</body>\n</html>";
        std::cout  << tmp;
    }
    else
    {
        std::ifstream bodytmp( (_server.getErrorpage() + "/" + statusCode.first + ".html"));
         for (std::string line; std::getline(bodytmp , line);) 
        {
            std::cout << line;
        }
    }
    return ("body.html");
}

void Cgi::setStatusCode(std::string code)
{
    statusCode.first = code;
    if (code == "200")
        statusCode.second = " OK";
    else if (code == "404")
        statusCode.second = " Not Found";
    else if (code == "403")
        statusCode.second = " Forbidden";
    else if (code == "201")
        statusCode.second = " Created";
    else if (code == "405")
        statusCode.second = " Not Allowed";
    else if (code == "301")
        statusCode.second = " Moved Permanently";
    else if (code == "302")
        statusCode.second = " found";
    else if (code == "400")
        statusCode.second = " Bad Request";
    else if (code == "413")
        statusCode.second = " Request Entity Too Large";
    else if (code == "500")
        statusCode.second = " Internal Server Error";
}

std::string Cgi::getHeader() const {return _header;}
std::string Cgi::getLocation() const {return _location;}
std::string Cgi::gettoRender_file() const{return toRender_file;}
