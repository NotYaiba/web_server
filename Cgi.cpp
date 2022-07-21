#include "Cgi.hpp"

Cgi::Cgi(Server serv , Request  req , Location const & loc) : _server(serv), _req(req), _loc(loc)
{

    initData();
    // exit(0);
}

void Cgi::initData()
{
    bool is_post = false;
    pid_t pid;
    int post_fd;
    if (_req.getMethod() == "POST")
        is_post = true; 
    cgimap = _server.getCgiMap();
    path = _req.getUri();
    path =   path.erase(path.size() - 1) ;
    filetype =  get_file_type(path);
    filepath =  removeRepeated(_loc.getRoot() + "/" +  path , '/');
    std::string def =  _loc.getDefaultt();
    if (def.size() > 1)
    {
        filepath = removeRepeated(filepath +"/" + def + "/", '/');
        filepath.erase(filepath.size() - 1);
    }

    int outfile_fd = open("./index.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
     post_fd = open(_req.getBody().c_str(),   O_RDONLY | O_TRUNC, 0666);
    //arguments
    char **arr = initarr();
    //envirement variables
    SetEnv();
    pid = fork();
        std::cout << "=========>fork \n";

    if (pid == 0)
    {
        if (is_post)
            dup2(post_fd, STDIN_FILENO);
        dup2(outfile_fd, STDOUT_FILENO);

        // std::cout << "=========>start excutr \n";
        if (execve(arr[0] ,arr, env) < 0)
        {

        }
        close(outfile_fd);
        // close(post_fd);
        // std::cout << "=========> end excutr \n";
            // throw "ERROR execve "; // TODO
            // std::cout << "dd\n";
    }
    waitpid(0, NULL, 0);
}
void Cgi::SetEnv()
{
    std::map<std::string , std::string> mp;
    std::vector<std::string> tmpv = split(filepath, "?");
    std::string query;
    if(tmpv.size() > 1)
    {
         query = tmpv[1];
        std::cout << "query string "<< query<< std::endl;
    }
    mp["REQUEST_METHOD"] = "GET";
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
    std::cout << "------ Location : " << _loc.getLocation() << std::endl;
    mp["PATH_INFO"] = "/wp-admin/setup-config.php";
    mp["REDIRECT_STATUS"] = "1";
    // mp["PATH_TRANSLATED"]a
    mp["QUERY_STRING"] = query;
    // mp["REMOTE_ADDR"]
    // mp["REMOTE_IDENT"]
    // mp["REMOTE_USER"]
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
    

    std::cout << "file path ===>" << filepath << std::endl;
    // if ()
	ar.push_back(cgimap["php"]);
	ar.push_back(filepath);

    return(vectToArr(ar));
}
