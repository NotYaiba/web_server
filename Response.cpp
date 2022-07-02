#include "Response.hpp"



Response::Response(Server  serv , Request req)
{
    std::cout << red << "\n\n start Response \n\n" << reset <<  std::endl;
    _server = serv;
    _req  = req;
    _path = _req.getUri();
    validMethod = 0;
    statusCode =  req.getStatusCode();
    // if (statusCode != 200 || statusCode != 201)
    // {
    //          // todo send response with status code; 
    // }
    // check 400 413 uri errors
    if (statusCode == 400 || statusCode == 413)
    {

    }
    findMatchingLocation();
    matching_location.debug();
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
    std::cout << "--------------"<<std::endl;

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
        std::cout << "location : " << loc << " | score : " << score  << " iterations : " << iterations << " |\n";
        
        std::pair<std::map<int ,int >::iterator , bool > ret = locScore.insert(std::make_pair(score, iterations));
        else if (ret.second == false)
        {
            if (iterations < (ret.first)->second)
            {
                if (ret=>first ==(--locScore.end())->first)
                {
                    
                }
                locScore[score] = iterations;
                matching_location = locations[i];
            }
        }
        if (score > (--locScore.end())->first )
            matching_location = locations[i];
    }

}

// void Response::getMethod()
// {
//     std::vector <std::string> v = split(_loc.getMethod(), " ");
//     for (size_t i = 0 ; v.size() ; i++)
//     {
//         if (_req.getMethod() == v[i])
//             validMethod = 1;
//     }
//     std::cout << validMethod << std::endl;
// }


