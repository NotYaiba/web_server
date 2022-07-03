#include "Response.hpp"



Response::Response(Server  serv , Request req)
{
    std::cout << red << "\n\n start Response \n\n" << reset <<  std::endl;
    _server = serv;
    _req  = req;
    _path = _req.getUri();
    _method  = _req.getMethod();
    validMethod = 0;
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

void   Response::findMatchingLocation()
 {
    int score = 0;

    int iterations = 0;
    std::map<int , int > locScore;
    locations = _server.getLocations();
    std::cout << _path <<std::endl;
    // std::cout << "--------------"<<std::endl;

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
        // std::cout << "location : " << loc << " | score : " << score  << " iterations : " << iterations << " |\n";
        
        std::pair<std::map<int ,int >::iterator , bool > ret = locScore.insert(std::make_pair(score, iterations));
        // std::cout << "max : " << (--locScore.end())->first << std::endl;
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
    // matching_location.debug();
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
            statusCode.first = 403;
        else
            statusCode.first = 200;
    }
    else
    {
       statusCode.first  = 404;
       statusCode.second  = " Not Found";

    }
}
void Response::Get()
{

}
void Response::Post()
{

}

void Response::generateHeader()
{   
    int len;
    if (statusCode.first== 404)
    {
            std::cout << "salam\n";
            body = generateBody();
    }
    else
        body = "";
    len = body.size();
    header += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
    header += "Content-type: text/html\r\n";
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
