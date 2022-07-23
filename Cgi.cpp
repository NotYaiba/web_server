#include "Cgi.hpp"

Cgi::Cgi(Server serv , Request  req , Location const & loc) : _server(serv), _req(req), _loc(loc)
{
    on = false;
    initData();
    // exit(0);
}

void Cgi::initData()
{
    _req.debug();
    bool is_post = false;
    pid_t pid;
    int post_fd;
    cgimap = _server.getCgiMap();
    path = _req.getUri();
    path =   path.erase(path.size() - 1) ;
    filepath =  removeRepeated(_loc.getRoot() + "/" +  path , '/');
    uri = _req.getUri();
    uri.erase(uri.size() - 1);
    method = _req.getMethod();
    if (method == "POST")
        is_post = true;
    std::cout << "--- is post : "  << is_post << std::endl;
    std::string def =  _loc.getDefaultt();
    if (def.size() > 1)
    {
        filepath = removeRepeated(filepath +"/" + def + "/", '/');
        filepath.erase(filepath.size() - 1);
    }
    std::vector<std::string> tmpv = split(filepath, "?");

    if(tmpv.size() > 1)
    {
        query = tmpv[1];
        filepath = tmpv[0];
    }
    filetype =  get_file_type(filepath);
    std::cout << "file path ===>" << filepath << std::endl;
    std::cout << "query string "<< query<< std::endl;
    if (isDirictory(filepath) ||( filetype != "application/x-php" && filetype != "application/x-python" ))
        return ;
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
    std::cout << yellow<< "DKHEL CGIII"<<  reset << std::endl;
    
    int outfile_fd = open("./index.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    // std::cout 
    post_fd = open(("tmp/" + _req.getBody()).c_str(),   O_RDONLY , 0666);
    // std::fstream fillle(_req.getBody());
    char buff[5000];
    int bytes = read(post_fd, buff, 5000);
    std::cerr << "===========>>>>";
    // std::cerr << fillle.rdbuf();
    std::cerr << "+++++" << bytes << std::endl;
    // fillle.close();
    // std::cout << _req.getBody() << std::endl;
    write(2, buff, bytes);
    std::cerr << "===========>>>>";
    //arguments
    char **arr = initarr();
    //envirement variables
    SetEnv();
    pid = fork();

    if (pid == 0)
        std::cout << "=========>fork child \n";
    else 
        std::cout << "=========>fork parent \n";
    if (pid == 0)
    {
        // int saved_stdout = dup(STDOUT_FILENO);
        if (is_post)
            dup2(post_fd, STDIN_FILENO);
        dup2(outfile_fd, STDOUT_FILENO);

        // close(outfile_fd);
        // if (is_post)
        //     close(post_fd);
        std::cerr << "=========>start excutr \n";
        // std::cout << "execve params : " << arr[0] << " --  " << arr[1] <<  std::endl;
        if (execve(arr[0] ,arr, env) < 0)
        {
            throw "ERROR execve "; // TODO
        }
        // exit(1);
        // dup2(saved_stdout, STDOUT_FILENO);
        // close(saved_stdout);
        // std::cout << "=========> end excutr \n";
            // std::cout << "dd\n";
    }
    close(outfile_fd);
    if (is_post)
    {
        close(post_fd);

    }
        std::cout <<reset << "=========> haniaa \n";
    // wait(NULL);
    waitpid(0, NULL, 0);
        std::cout <<reset << "=========> haniaaaaa \n";
    pars_file();

}
void Cgi::SetEnv()
{
    std::map<std::string , std::string> mp;
    
    mp["REQUEST_METHOD"] = method;
    mp["SERVER_PROTOCOL"] = "HTTP/1.1";
    mp["CONTENT_TYPE"] = filetype;
    mp["CONTENT_LENGTH"] = std::to_string(_req.getContentLength());
    mp["SERVER_PORT"] = std::to_string(_server.getPort());
    mp["SERVER_NAME"] = (_server.getServerName())[0];
    mp["REMOTE_HOST"] = _server.getHost();
    // mp["AUTH_TYPE"]
    // mp["GATEWAY_INTERFACE"]
    {
        std::vector <std::string> tmp = split(path, "/");
        std::string path_info = "/";
        for (std::vector<std::string>::iterator it = tmp.begin(); it != --tmp.end(); it++)
            path_info += *it + "/";
    }
    //TODO : fill the variables bellow dinammically
    // mp["PATH_INFO"] = uri;
    mp["PATH_INFO"] = "mohamed";
    mp["REDIRECT_STATUS"] = "1";
    // mp["PATH_TRANSLATED"]a
    std::cout << "queryyyy ----->|" << query << "|" << std::endl;
    std::cout << "uriiiiii ----->|" << uri << "|" << std::endl;
    mp["QUERY_STRING"] = query;
    // mp["QUERY_STRING"] = "step=1";
    // mp["REMOTE_ADDR"]
    // mp["REMOTE_IDENT"]
    // mp["REMOTE_USER"]
    size_t i = path.find("?");
    if (i != std::string::npos)
        path.erase(i);
    std::cout << "pathhhhh ----->|" << path << "|" << std::endl;
    mp["SCRIPT_NAME"] = path;
    // mp["SCRIPT_FILENAME"] = "/Users/aez-zaou/Desktop/wordpress/index.php";
    mp["SCRIPT_FILENAME"] = removeRepeated(_loc.getRoot() + "/" +  path , '/');
    // mp["HTTP_HOST"] = "127.0.0.1:8000";
    mp["HTTP_HOST"] = _server.getHost() + ":" + std::to_string(_server.getPort());
    // mp["SERVER_SOFTWARE"]
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
    

    std::cout <<"|"<< cgikey <<  "|"<<std::endl;
	ar.push_back(cgikey);
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

     std::cout << "toRender_file: "<< toRender_file << std::endl;
    std::ifstream MyReadFile("index.html");
    _header = "";
    for (std::string line; std::getline(MyReadFile, line);) 
    {
        std::cout << line << std::endl;
        if (line == "\r")
            break;
        std::string tmp;
        if ((tmp = find("Location", line)) != "")
        {
            _location = tmp;
            continue;
        }
        if ((tmp = find("Status", line)) != "")
        {
            _status = tmp;
            continue;
        }
        _header += line + "\r\n";
    }
    if (_status.size())
    {
        std::vector<std::string> v = split(_status, ":");
        v = split(v[1], " ");
        _status = v[0];
    }
    if (_location.size())
    {
        std::vector<std::string> v = split(_location, ": ");
        _location = v[1];
    }
    int newfile_fd = open("index1.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
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
    return -1;
}
std::string Cgi::getHeader() const {return _header;}
std::string Cgi::getLocation() const {return _location;}

std::string Cgi::gettoRender_file() const{return "index1.html";}
