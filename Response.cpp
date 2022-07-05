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
            setStatusCode(403);
        else
            setStatusCode(200);
    }
    else
        setStatusCode(404);

}

void Response::Get()
{
    setStatusCode(405);
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
    std::cout << blue << new_file << reset<<std::endl;
    std::string file_name = removeRepeated(matching_location.getUpload() + "/" + new_file, '/');
    std::string tmp = "./tmp/" + new_file;
    std::ifstream in(tmp.c_str(), std::ios::in | std::ios::binary);
    std::ofstream out(file_name, std::ios::out | std::ios::binary);
    out << in.rdbuf();
    in.close();
    out.close();
    std::cout << "here" << std::endl;
    remove(_req.getBody().c_str());
    setStatusCode(201);
}

void Response::generateHeader()
{   
    int len;
    body = generateBody();
    len = body.size();
    header += "HTTP/1.1 " + std::to_string(statusCode.first) + statusCode.second + "\r\n" ;
    header += "Content-type:  text/html\r\n";
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
        statusCode.second = " leeda";
}