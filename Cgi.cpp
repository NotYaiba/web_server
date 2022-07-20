#include "Cgi.hpp"

Cgi::Cgi(Server serv , Request  req , Location const & loc) : _server(serv), _req(req), _loc(loc)
{

    initData();
}

void Cgi::initData()
{
    bool is_post = false;
    pid_t pid;
    int post_fd;

    cgimap = _server.getCgiMap();
    path = _req.getUri();
    path =   path.erase(path.size() - 1) ;
    filetype =  get_file_type(path);

    int outfile_fd = open("./index.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
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
        std::cout << "=========>dup out excutr \n";

        dup2(outfile_fd, STDOUT_FILENO);
        std::cout << "=========>close  out excutr \n";

        // close(post_fd);
        // close(outfile_fd);
        std::cout << "=========>start excutr \n";
        if (execve(arr[0] ,arr, env) < 0)
        {

        }
        std::cout << "=========> end excutr \n";
            // throw "ERROR execve "; // TODO
            // std::cout << "dd\n";
    }
    waitpid(0, NULL, 0);
}
void Cgi::SetEnv()
{
    std::map<std::string , std::string> mp;
    mp["REQUEST_METHOD"] = "GET";
    mp["SERVER_PROTOCOL"] = "HTTP/1.1";
    mp["SERVER_"] = "hole";
    mp["CONTENT_TYPE"] = filetype;
    std::vector<std::string > v;
    for ( std::map<std::string , std::string>::iterator it = mp.begin() ; it != mp.end(); it++)
    {
        std::string tmp = it->first + "=" + it->second;
        std::cout << tmp << std::endl;
        v.push_back(tmp);
    }
	env = vectToArr(v);
	// env.push_back((char*)"REQUEST_METHOD=GET");
	// env.push_back((char*)"SERVER_PROTOCOL=HTTP/1.1");
	// env.push_back((char*)"SERVER_=hola");
	// env.push_back((char*)"CONTENT_TYPE=hola");
	// env.push_back((char*)std::string("CONTENT_TYPE=" + filetype ).c_str() );
	// env.push_back((char*)("CONTENT_TYPE=" + filetype ) );

}
char **  Cgi::initarr()
{
    std::vector<std::string> ar;
    
    std::string filepath =  removeRepeated(_loc.getRoot() + "/" +  path , '/');
    std::cout << "file path ===>" << filepath << std::endl;
    // if ()
	ar.push_back(cgimap["python"]);
	ar.push_back(filepath);

    return(vectToArr(ar));
}