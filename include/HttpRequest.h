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
 * @file HttpRequest.h
 * 
 ***************************************************************************/

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
    // void setMethod(const std::string& method);
    std::string sendRequest();

private:
    void clearRequest();
    std::vector<std::string> m_headerList;
    std::string m_url;
    std::string m_data;
    // std::string m_method;
};