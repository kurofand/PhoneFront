#include "cli.hpp"
#include "../Log/log.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <list>
#include <sstream>

bool Cli::getNumInfo()
{
    std::stringstream ss;

    std::list<std::string> headers;
    headers.push_back("User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0");
    std::string url="https://www.jpnumber.com/searchnumber.do?number="+*number;
    try
    {
        curlpp::Easy request;
        request.setOpt(curlpp::options::Url(url));
        request.setOpt(curlpp::options::HttpHeader(headers));

        std::ostringstream os;
        request.perform();
        os<<request;
        std::cout<<os.str()<<std::endl;
    }
    catch(std::exception &e)
    {
        Log(LogLevel::Error)<<"Error on get request. Exception: ["<<e.what()<<"]";
        return false;
    }

    return true;
}
