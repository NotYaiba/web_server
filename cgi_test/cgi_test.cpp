
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
 #include <fcntl.h>
#include <vector>

extern char **environ;

void set_env()
{
    setenv("REQUEST_METHOD","GET",1);
    setenv("SERVER_PROTOCOL","HTTP/1.1",1);
    setenv("PATH_INFO","/Users/aez-zaou/Desktop/cgi/script.php",1);
    setenv("SERVER_NAME","holaaa",1);
    setenv("REQUEST_METHOD","GET",1);
    // setenv("REQUEST_METHOD","GET",1);
    // setenv("REQUEST_METHOD","GET",1);
    // setenv("REQUEST_METHOD","GET",1);
}

int main (int ac, char** av)
{
    (void)ac;
    (void)av;
    pid_t pid;
    int post_fd;
    bool is_post = false;
    char *args[3];
    int outfile_fd = open("./index.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);

    //arguments
    std::vector<char*> ar;
	ar.push_back((char*)"/Users/aez-zaou/.brew/bin/php-cgi");
	ar.push_back((char*)"./script.php");
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

    return 0;
}