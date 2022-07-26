#include "Cgi.hpp"

Cgi::Cgi(Server serv , Request  req , Location const & loc) : _server(serv), _req(req), _loc(loc)
{
    toRender_file= "index-" + random_string();
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
    path = _req.getUri();
    path =   path.erase(path.size() - 1) ;
    filepath =  removeRepeated(_loc.getRoot() + "/" +  path , '/');

    std::cout << "matching Location: " <<_loc.getLocation() << std::endl;
    std::string def =  _loc.getDefaultt();

    if (def.size() > 1 && isDirictory(filepath))
    {
        std::cout << " def: " <<def << std::endl;
        filepath = removeRepeated(filepath +"/" + def + "/", '/');
        filepath.erase(filepath.size() - 1);
        std::cout << "default path ; " << filepath << std::endl;
    }
    std::vector<std::string> tmpv = split(filepath, "?");
    if(tmpv.size() > 1)
    {
        query = tmpv[1];
        filepath = tmpv[0];
    }
    uri = _req.getUri();
    uri.erase(uri.size() - 1);
    method = _req.getMethod();
    filetype =  get_file_type(filepath);

    if (method == "POST")
        is_post = true;

    std::cout <<blue <<"hellow cgi is on " << on << reset << std::endl;
    if (isDirictory(filepath) ||( filetype != "application/x-php" && filetype != "application/x-python" ))
    {
    std::cout <<blue <<"hellow cgi is on " << on << reset << std::endl;

        return ;
    }
    else
    {
        if (filetype == "application/x-php")
            cgikey = cgimap["php"];
        else
            cgikey = cgimap["python"];
        if (cgikey == "")
            throw "BAD GET AWAY !"; // TODO
        on = true;
    }
}

void Cgi::execute_cgi()
{

    pid_t pid;
    int post_fd;
    std::cout << "file path ===>" << filepath << std::endl;
    std::cout << "query string "<< query<< std::endl;
    std::cout << yellow<< "DKHEL CGIII"<<  reset << std::endl;
    
    int outfile_fd = open("./index.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    // std::cout 
    // dupp_file("tmp/" + _req.getBody());
    post_fd = open(("tmp/" + _req.getBody()).c_str(),   O_RDONLY , 0666);
    // post_fd = open("new_file",   O_RDONLY , 0666);
    if (post_fd == -1)
    {
        std::cerr << "error open file " << "tmp/" + _req.getBody() << std::endl;
    }
    arr = initarr();
    SetEnv();
    pid = fork();

    if (pid == 0)
    {
        if (is_post)
        {
            if (dup2(post_fd, STDIN_FILENO) == -1)
                std::cerr << "error dup2 file " << "tmp/" + _req.getBody() << std::endl;
        }
        if (dup2(outfile_fd, STDOUT_FILENO) == -1)
            std::cerr << "error dup2 file out file " << std::endl;
        close(outfile_fd);
        if (is_post)
            close(post_fd);
        if (execve(arr[0] ,arr, env) < 0)
        {
            perror ("ERROR execve "); // TODO
        }
    }
    close(outfile_fd);
    if (is_post)
        close(post_fd);
    while (wait(NULL) > 0);
}
void Cgi::SetEnv()
{
    std::map<std::string , std::string> mp;
    std::map<std::string , std::string> req_headers = _req.getHeaders();
    for(std::map<std::string , std::string>::iterator it = req_headers.begin() ; it != req_headers.end() ; it++ )
    {
        std::string key("HTTP_");
        key += it->first;
        std::transform(key.begin(), key.end(), key.begin(), asciiToUpper);
        mp[key] = it->second;
    }
    
    mp["REQUEST_METHOD"] = method;
    mp["SERVER_PROTOCOL"] = "HTTP/1.1";
    mp["GATEWAY_INTERFACE"] = "CGI/1.1";
    mp["CONTENT_TYPE"] = _req.getContentType();
    mp["CONTENT_LENGTH"] = std::to_string(_req.getContentLength());
    mp["SERVER_PORT"] = std::to_string(_server.getPort());
    mp["SERVER_NAME"] = (_server.getServerName())[0];
    // mp["REMOTE_ADDR"] = _server.getHost();
    // {
    //     std::vector <std::string> tmp = split(path, "/");
    //     std::string path_info = "/";
    //     for (std::vector<std::string>::iterator it = tmp.begin(); it != --tmp.end(); it++)
    //         path_info += *it + "/";
    // }
    //TODO : fill the variables bellow dinammically
    // std::cout << filepath << " <-- filepath | path " << path;
    // std::cout << uri << " <-- uri /\n";
    mp["PATH_INFO"] = _loc.getDefaultt();

    mp["PATH_TRANSLATED"] = filepath;
    // mp["REQUEST_URI"] = uri;
    mp["REDIRECT_STATUS"] = "200";
  
    mp["QUERY_STRING"] = query;
    mp["SCRIPT_NAME"] = path;
    mp["DOCUMENT_ROOT"] = _loc.getRoot();
    mp["DOCUMENT_URI"] = filepath;


    mp["HTTP_COOKIE"] = _req.getCookie();
    std::vector<std::string> v;
    for ( std::map<std::string , std::string>::iterator it = mp.begin() ; it != mp.end(); it++)
    {
        std::string tmp = it->first + "=" + it->second;
        v.push_back(tmp);
    }
	env = vectToArr(v);
}
char **  Cgi::initarr()
{
    std::vector<std::string> ar;
	ar.push_back(cgikey);
    std::cerr << filepath << std::endl;
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
    //  std::cout << "ex: "<<  << std::endl;
    std::ifstream MyReadFile("index.html");

    _header += "Server: mywebserver\r\n";

    _header += "Access-Control-Allow-Origin: *\r\n";
    _header += "Access-Control-Allow-Private-Network: true\r\n";
    for (std::string line; std::getline(MyReadFile, line);) 
    {
        std::cout << line << std::endl;
        if (line == "\r")
            break;
        std::string tmp;
        if ((tmp = find("Location", line)) != "")
        {
            _location = tmp;
            // continue;
        }
        if ((tmp = find("Status", line)) != "")
        {
            _status = tmp;
            continue;
        }
        _header += line + "\r\n";
    }

    // _header += "Access-Control-Expose-Headers: Set-Cookie\r\n";
    if (_status.size())
    {
        std::vector<std::string> v = split(_status, ":");
        // v = split(v[1], " ");
         _status = v[1];
        _header = "HTTP/1.1 "+ _status + _header + "\r\n";
    }
    else
        _header = "HTTP/1.1 200 OK\n\r"  + _header + "\r\n";
    if (_location.size())
    {
        std::vector<std::string> v = split(_location, ": ");
        _location = v[1];
    }

    int newfile_fd = open(toRender_file.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(newfile_fd, STDOUT_FILENO);
    for (std::string line; std::getline(MyReadFile, line);) 
    {
        std::cout << line;
    }
    std::cout << std::endl;
    close(newfile_fd);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    std::cout << "----------------header----------------" << std::endl;
    std::cout << yellow << _header << reset;
    std::cout << _location << std::endl;
    std::cout << "|" << _status << "|" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}

int Cgi::getStatus() const 
{
    if (_status.size())
        return std::stoi(_status);
    return 200;
}
std::string Cgi::getHeader() const {return _header;}
std::string Cgi::getLocation() const {return _location;}

std::string Cgi::gettoRender_file() const{return toRender_file;}

void Cgi::dupp_file(std::string filename)
{
    std::ifstream MyReadFile(filename.c_str());
    int newfile_fd = open("new_file", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(newfile_fd, STDOUT_FILENO);
    for (std::string line; std::getline(MyReadFile, line);) 
    {
        std::cout << line;
        std::cerr << line;
    }
    close(newfile_fd);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

}
// std::string Cgi::generateBody()
// {
//   std::ofstream bodytmp("body.html", std::ios::out | std::ios::binary);
//     if (_server.getErrorpage() == "")
//     {
//         std::string msg = std::to_string(statusCode.first) +  statusCode.second;
//         std::string tmp;
//         tmp = "<html>\n<head><title>" + msg + "</title></head>\n<body bgcolor='white'>\n<center><h1>"  +msg + "</h1></center>\n</body>\n</html>";
//         bodytmp << tmp;
//         file_name = "body.html";
//         file_size = tmp.size();
//         file_type = "text/html";
//         return "body";
//     }
//     else
//     {
//         file_name = (_server.getErrorpage() + "/" + std::to_string(statusCode.first) + ".html");
//         file_size = fsize(file_name.c_str());
//         file_type = "text/html";
//     }
//     return ("");
// }