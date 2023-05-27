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

#include <vector>
#include <cstring>

#include "Plug.h"
#include "Soap.h"
#include "HttpRequest.h"
#include "json.hpp"
using json = nlohmann::json;

Plug::Plug() {
}

Plug::~Plug() {
}

void Plug::setDeviceIP(const std::string& ip)
{
    m_devIP = ip;
}

std::string Plug::getDeviceIp() 
{
    return m_devIP;
}

std::string Plug::getBasicEvents(const std::string& wMethod) {
    
    std::string wServiceType = "urn:Belkin:service:basicevent:1";
    std::string soap = createSoapRequest(wMethod, wServiceType);
    std::string url = "http://wemo:49153/upnp/control/basicevent1";    

    HttpRequest http;
    http.setUrl(url);

    http.setHeaders("SOAPACTION", "\"" + wServiceType + "#" + wMethod + "\"");
    http.setHeaders("Content-Length", std::to_string(soap.length()));

    http.setData(soap);

    std::string response = http.sendRequest();
    //printf("%s\n", response.c_str());
    return response;
}


std::string Plug::setBinaryEvents(const std::string& wMethod, 
    const std::string& wName, const std::string& wState) {
    
    std::string wServiceType = "urn:Belkin:service:basicevent:1";
    std::string soap = createSoapRequest(wMethod, wServiceType, wName, wState);

    std::string url = "http://wemo:49153/upnp/control/basicevent1";    
    HttpRequest http;
    http.setUrl(url);

    http.setHeaders("SOAPACTION", "\"" + wServiceType + "#" + wMethod + "\"");
    // http.setHeaders("SOAPACTION", "\"urn:Belkin:service:basicevent:1#SetBinaryState\"");
    http.setHeaders("Content-Length", std::to_string(soap.length()));

    http.setData(soap);

    std::string response = http.sendRequest();
    //printf("%s\n", response.c_str());
    return response;
}

std::string Plug::getState() {
    std::string state = parseSoapResponse(getBasicEvents("GetBinaryState"), "GetBinaryStateResponse");

    json root = json::parse(state);    
    if (root.find("BinaryState") != root.end()) {
        root["State"] = (root.at("BinaryState").get<std::string>() == "1") ?  "On" : "Off";
        root.erase("BinaryState");
    }
    root["IP"] = m_devIP;
    return root.dump(4);
}

std::string Plug::setState(const std::string& state) {
    std::string ret = parseSoapResponse(setBinaryEvents("SetBinaryState", "BinaryState", state.c_str()), 
        "SetBinaryStateResponse");
    
    json root = json::parse(ret);
    if (root.find("BinaryState") != root.end()) {
        auto status = root.at("BinaryState").get<std::string>();
        if (status == "1")
            root["State"] = "On";
        else if (status == "0")
            root["State"] = "Off";
        else
            root["State"] = "Unknown";
        root.erase("BinaryState");
    }
    root["IP"] = m_devIP;
    return root.dump(4);
}

std::string Plug::getDeviceInfo() {
    std::string mac = parseSoapResponse(getBasicEvents("GetMacAddr"), "GetMacAddrResponse");
    std::string homeId = parseSoapResponse(getBasicEvents("GetHomeId"), "GetHomeIdResponse");
    std::string name = parseSoapResponse(getBasicEvents("GetFriendlyName"), "GetFriendlyNameResponse");
    std::string env = parseSoapResponse(getBasicEvents("GetServerEnvironment"), "GetServerEnvironmentResponse");

    json root = json::parse(mac);
    root.merge_patch(json::parse(homeId));
    root.merge_patch(json::parse(name));
    root.merge_patch(json::parse(env));
    root["IP"] = m_devIP;
    return root.dump(4); 
}
