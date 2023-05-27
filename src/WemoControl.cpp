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
 * @file WemoControl.cpp
 * 
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <clocale>

#include "Log.h"
#include "WemoControl.h"
using namespace std;


void printUsage()
{
    std::cout << "\n"
                "Usage: wemocontrol {Options} [Commands] {Args}\n"
                "Options:\n"
                "  --help\t\t\tShow the usage menu of this app\n"
                "  --verbose\t\t\tEnable verbose logs\n"
                << std::endl;

    std::cout << "Commands:\n"
                "  off\t\t\t\tTurn Off Wemo device\n"
                "  on\t\t\t\tTurn On Wemo device\n"
                "  status\t\t\tGet the current status of Wemo device\n"
                "  getdeviceinfo\t\t\tGet the device information\n"
                << std::endl;
}

bool WemoControl::checkArgOptions(const std::vector<std::string>& args, const std::string& cmd, 
        const std::string& option, int& param)
{
    std::string strParam;
    auto ret = checkArgOptions(args, cmd, option, strParam);
    try {
        param = std::stoi(strParam);
    }
    catch (exception& e) {
        LOG_D("Exception @ %s", e.what());
    }
    return ret;
}

bool WemoControl::checkArgOptions(const std::vector<std::string>& args, const std::string& cmd, 
    const std::string& option,  std::string& param)
{
    auto it = std::find(args.begin(), args.end(), option);
    if (it != args.end()) {
        if (std::next(it) != args.end()) {
            param = *(std::next(it));
            return true;
        }
        else {
            std::cerr << "Error: Missing required parameter for " << option << " option\n";
            return false;
        }
    }
    else {
        std::cerr << "Unknown Option \n\n"
            "Usage: wemodeviceapp " << cmd << " {OPTIONS}\n"
            "Try 'wemodeviceapp " << cmd << " --help' for help."
            << std::endl;
        return false;
    }
    return true;
}

bool WemoControl::validateArgsUsage(const std::vector<std::string>& args) 
{
    if (args.empty() ||  g_cmdMap.find(args.at(0)) == g_cmdMap.end()) {
        printUsage();
        return false;
    }

    bool showArgUsage = false;
    std::string ipAddr, ret, cmd = args.at(0);
    std::vector<std::string> argUsageList;
    auto eCmd = g_cmdMap.at(cmd);

    if (args.at(1) == "--help") {
        showArgUsage = true;
        argUsageList.emplace_back("--ip\t\t\tIP address of the plug.\n");
        argUsageList.emplace_back("--help\t\t\tShow this message and exit.\n");
    }
    else {
        if (!checkArgOptions(args, cmd, "--ip", ipAddr))
            return false;
        m_plug.setDeviceIP(ipAddr);
    }
    
    switch (eCmd)
    {		
	case on:
	case off:
	case status:
    case getdeviceinfo:
        if (!showArgUsage)
            performWemoRequest(cmd);
    break;
	// case setbrightness:
    //     if (showArgUsage)
    //         argUsageList.emplace_back("--dim\t\t\tBrightness level in percent [0 to 100].\n");
    //     else {
    //         int dimlevel;
    //         if (checkArgOptions(args, cmd, "--dim", dimlevel))
    //             ret = m_plug.setBrightness(dimlevel);
    //     }
    // break;

	default:
        printUsage();
	    return false;
    }

    if (showArgUsage) {
        std::cout << "Usage: wemodeviceapp "<< cmd << " {OPTIONS}\n\n"
                "Options:" << std::endl;
        for (auto& it : argUsageList)
            std::cout << it;
        return false;
    }
    else if (!ret.empty())
        cout << ret << endl;
    return true;
}

WemoControl& WemoControl::getInstance() {
    static WemoControl instance;
    return instance;
}

WemoControl::WemoControl() 
{
    g_cmdMap = {
        {"on", on},
        {"off", off},
        {"status", status},
        {"getdeviceinfo", getdeviceinfo}
    };
}

WemoControl::~WemoControl() {
}

bool WemoControl::isCmdSupported(const std::string& cmd) {
    auto it = g_cmdMap.find(cmd);

    if (it != g_cmdMap.end())
        return true;
    else
        return false;
}

std::string WemoControl::performWemoRequest(const std::string& cmd)
{
    std::string ret;
    if (g_cmdMap.find(cmd) == g_cmdMap.end()) {
        printUsage();
        return ret;
    }
    auto eCmd = g_cmdMap.at(cmd);

    if (m_plug.getDeviceIp().empty()) {
        std::string ip;
        while (1) {
            cout << "Enter the plug IP address: ";
            getline(cin, ip);
            if (ip.empty())
                continue;
            else
                break;
        }
        m_plug.setDeviceIP(ip);
    }

    switch (eCmd) {
        case on:
            ret = m_plug.setState("1");
        break;

        case off:
            ret = m_plug.setState("0");
        break;

        case status:
            ret = m_plug.getState();
        break;

        case getdeviceinfo:
            ret = m_plug.getDeviceInfo();
        break;

	    // case setbrightness:
        //     ushort level;
        //     cout << "Enter the brightness level [0 to 100]:";
        //     cin >> level;
        //     ret = m_plug.setBrightness(level);
        // break;

        default:
        break;
    }
    if (!ret.empty())
        cout << ret << endl;

    return ret; 
}

int main(int argc, char *argv[])
{
    WemoControl& wemo = WemoControl::getInstance();
    if (argc == 1) {
        printUsage();
        return 0;
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }

    auto hIt = std::find(args.begin(), args.end(), "--help");
    if ((hIt != args.end()) && (hIt == args.begin())) {
        printUsage();
        return 0;
    }

    auto vIt = std::find(args.begin(), args.end(), "--verbose");
    if (vIt != args.end()) {
        L::setLogLevel(L::d);
        LOG_D("Verbose log enabled");
        args.erase(vIt);
    }

    if (!wemo.isCmdSupported(args.at(0))) {
        printUsage();
        return 0;
    }

    if (args.size() == 1)
        wemo.performWemoRequest(args.at(0));
    else
        wemo.validateArgsUsage(args);
}
