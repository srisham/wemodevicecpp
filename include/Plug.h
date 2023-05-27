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
 * @file Plug.cpp
 * 
 ***************************************************************************/

#include <iostream>

class Plug {

public:
    Plug();
    ~Plug();

    void setDeviceIP(const std::string& ip);
    std::string getDeviceIp();

    std::string getState();
    std::string setState(const std::string& state);
    std::string getDeviceInfo();

private:
    std::string getBasicEvents(const std::string& wMethod);
    std::string setBinaryEvents(const std::string& wMethod, 
        const std::string& wName, const std::string& wState);

    std::string m_devIP;
};

