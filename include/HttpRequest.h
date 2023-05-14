#pragma once
#include <iostream>
#include <vector>

class HttpRequest {

public:
    HttpRequest();
    ~HttpRequest();

    void setUrl(const std::string& url);
    void setHeaders(const std::string& key, const std::string& value);
    void setData(const std::string& data);
    std::string sendRequest();

private:
    void clearRequest();
    std::vector<std::string> m_headerList;
    std::string m_url;
    std::string m_data;
};