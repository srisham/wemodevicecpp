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
 * @file WemoControl.h
 * 
 ***************************************************************************/

#include <iostream>
#include <map>
#include <vector>
#include "Plug.h"


class WemoControl
{
public:
    static WemoControl& getInstance();
    bool isCmdSupported(const std::string& cmd);
    bool validateArgsUsage(const std::vector<std::string>& args);
    std::string performWemoRequest(const std::string& cmd);


private:
    bool checkArgOptions(const std::vector<std::string>& args, const std::string& cmd, 
        const std::string& option, std::string& param);
    bool checkArgOptions(const std::vector<std::string>& args, const std::string& cmd, 
        const std::string& option, int& param);

    WemoControl();
    ~WemoControl();

    enum WEMOCMD {
        on, off, status,getdeviceinfo
    };
    Plug m_plug;
    std::map<std::string, WEMOCMD> g_cmdMap;
};