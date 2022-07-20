#include "Cgi.hpp"

Cgi::Cgi(Server serv , Request  req , Location loc)
{

    initData(serv , req , loc);
}

void Cgi::initData(Server serv , Request  req ,  Location loc)
{
    _server = serv;
    _req = req;
    cgimap = _server.getCgiMap();
    std::string path =  removeRepeated(_loc.getRoot() + "/" + _req.getUri(), '/');
    std::cout << blue << "===> "<< _loc.getRoot() << reset << std::endl;
    std::cout << blue << path.erase(path.size()) << reset << std::endl;
    pid_t pid;
    int post_fd;
    bool is_post = false;
    char *args[3];
    int outfile_fd = open("./index.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);


    //arguments
    std::vector<char*> ar;
	ar.push_back((char*)"/usr/bin/python");
	ar.push_back((char*)"cgi_test/script.py");
	ar.push_back(0);

    //envirement variables
    std::vector<char*> env;
	env.push_back((char*)"REQUEST_METHOD=GET");
	env.push_back((char*)"SERVER_PROTOCOL=HTTP/1.1");
	env.push_back((char*)"SERVER_=hola");
	env.push_back(0);

    // args[0] = "/usr/bin/python3";
    // args[0] = "/usr/bin/php";
    // args[1] = "./script.php";
    // args[1] = NULL;
    // args[2] = NULL;
    pid = fork();

    if (pid == 0)
    {
        //set_env();
        if (is_post)
            dup2(post_fd, STDIN_FILENO);
        dup2(outfile_fd, STDOUT_FILENO);
        close(post_fd);
        execve(ar[0], &ar.front(), &env.front());
    }
    waitpid(0, NULL, 0);
    std::cout << "hole\n";

}