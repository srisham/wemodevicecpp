#include "HttpRequest.h"
#include <curl/curl.h>

struct response {
  char *memory;
  size_t size;
};

// static size_t
// mem_cb(void *contents, size_t size, size_t nmemb, void *userp)
// {
//   size_t realsize = size * nmemb;
//   struct response *mem = (struct response *)userp;

//   char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
//   if(!ptr) {
//     /* out of memory! */
//     printf("not enough memory (realloc returned NULL)\n");
//     return 0;
//   }

//   mem->memory = ptr;
//   memcpy(&(mem->memory[mem->size]), contents, realsize);
//   mem->size += realsize;
//   mem->memory[mem->size] = 0;

//   return realsize;
// }


HttpRequest::HttpRequest() {
    setHeaders("Content-type", "text/xml; charset=utf-8");
    setHeaders("Connection", "keep-alive");
}

HttpRequest::~HttpRequest() {

}

void HttpRequest::setUrl(const std::string& url) {
    m_url = url;
    printf("URL %s\n", m_url.c_str());
}

void HttpRequest::setHeaders(const std::string& key, const std::string& value) {
    std::string header = key + ": " + value;
    m_headerList.emplace_back(header);
    printf("%s\n", header.c_str());
}
void HttpRequest::setData(const std::string& data) {
    m_data = data;
    printf("%s\n", m_data.c_str());
}

// void HttpRequest::setMethod(const std::string& method) {
//     m_method = method;    
// }

std::string HttpRequest::sendRequest() {
    CURL *curl;
    CURLcode res;
    std::string response;
    // std::string url = "http://wemo:49153/upnp/control/basicevent1";

    curl = curl_easy_init();
    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());

        struct curl_slist* headers = NULL;
        for (const auto& hdr : m_headerList) {
            headers = curl_slist_append(headers, hdr.c_str());    
        }
        if (m_headerList.size() > 0) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
        
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); /* redirects! */
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); /* no file */
        
        if (!m_data.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_data.c_str());
        }
        
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        // else {
        // /*
        // * After the login POST, we have received the new cookies. Switch
        // * over to a GET and ask for the login-protected URL.
        // */
        // curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/file");
        // curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L); /* no more POST */
        // res = curl_easy_perform(curl);
        // /* Check for errors */
        // if(res != CURLE_OK)
        //     fprintf(stderr, "second curl_easy_perform() failed: %s\n",
        //             curl_easy_strerror(res));
        // }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return response;
}

