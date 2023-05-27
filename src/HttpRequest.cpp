/***************************************************************************
 *  Project                WEMODEVICECPP     
 *
 * Copyright (C) 2023 , Sri Balaji S.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the LICENSE file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * @file HttpRequest.cpp
 * 
 ***************************************************************************/

#include "HttpRequest.h"
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

static std::string s_cResponse;

struct memory {
    char *response;
    size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)clientp;

    char *ptr = (char *)realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
    return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    s_cResponse = mem->response;
    return realsize;
}

HttpRequest::HttpRequest() {
    setHeaders("Content-type", "text/xml; charset=utf-8");
    setHeaders("Connection", "keep-alive");
}

HttpRequest::~HttpRequest() {

}

void HttpRequest::setUrl(const std::string& url) {
    m_url = url;
    // printf("URL %s\n", m_url.c_str());
}

void HttpRequest::setHeaders(const std::string& key, const std::string& value) {
    std::string header = key + ": " + value;
    m_headerList.emplace_back(header);
    // printf("%s\n", header.c_str());
}
void HttpRequest::setData(const std::string& data) {
    m_data = data;
    // printf("%s\n", m_data.c_str());
}

// void HttpRequest::setMethod(const std::string& method) {
//     m_method = method;    
// }

std::string HttpRequest::sendRequest() {
    CURL *curl;
    CURLcode res;
    std::string response;
    struct memory chunk = {0};
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

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        
        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    response = s_cResponse;
    // printf("Response:\n %s\n\n", response.c_str());
    return response;
}

