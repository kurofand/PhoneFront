#include "cli.hpp"
#include "../Log/log.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include <list>
#include <sstream>
#include <locale>
#include <codecvt>

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
        request.setOpt(curlpp::options::WriteStream(&ss));
        request.perform();
        auto responseCode=curlpp::infos::ResponseCode::get(request);
        if(responseCode!=200)
        {
            Log(LogLevel::Error)<<"Response code 200 expected, "<<responseCode<<" received";
            return false;
        }
    }
    catch(const std::exception &e)
    {
        Log(LogLevel::Error)<<"Error on get request. Exception: ["<<e.what()<<"]";
        return false;
    }

    auto *doc=htmlReadMemory(ss.str().c_str(), ss.str().length(), "", "UTF-8", HTML_PARSE_RECOVER);
    if(!doc)
    {
        Log(LogLevel::Error)<<"Failed to get document instance from stream";
        return false;
    }

    auto *root=xmlDocGetRootElement(doc);
    if(!root)
    {
        Log(LogLevel::Error)<<"Failed to get root element";
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }

    auto *node=findNode(root, "div", "id", "result-main-right");
    if(!node)
    {
        Log(LogLevel::Error)<<"Failed to get node element: div, id: result-main-right";
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }

    node=findNode(node, "div", "class", "content");
    if(!node)
    {
        Log(LogLevel::Error)<<"Failed to get node element: div, class: content";
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }

    std::string s;
    try
    {
        node=node->children->next->next->next;
        auto *content=xmlNodeGetContent(node);
        s=reinterpret_cast<char*>(content);
        xmlFree(content);
    }
    catch(const std::exception &e)
    {
        Log(LogLevel::Error)<<"Exception on getting node content. Exception: ["<<e.what()<<"]";
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }
    if(!s.empty())
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring ws=converter.from_bytes(s);
        auto pos=ws.find(L"：");
        if(pos!=std::wstring::npos)
            name_=converter.to_bytes(ws.substr(pos+1));
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();

    if(name_.empty()||name_=="未登録")
        name_="Not registered";

    return true;
}

xmlNode* Cli::findNode(xmlNode *node, const char* tag, const char* propName, const char* propVal)
{
    xmlNode *res=nullptr;

    for(;node!=nullptr;node=node->next)
    {
        if(node->type==XML_ELEMENT_NODE)
        {
            if(!xmlStrcmp(node->name, (xmlChar*)tag))
                if(propName&&propVal)
                {
                    auto *val=xmlGetProp(node, (xmlChar*)propName);
                    if(val&&!xmlStrcmp(val, (xmlChar*)propVal))
                    {
                        xmlFree(val);
                        return node;
                    }
                    xmlFree(val);
                }
                else
                    return node;
            res=findNode(node->children, tag, propName, propVal);
            if(res)
                break;
        }
    }

    return res;
}
