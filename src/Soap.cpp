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
 * @file Soap.h
 * 
 ***************************************************************************/


#include <pugixml.hpp>
#include <sstream>
#include "json.hpp"
#include "Soap.h"

using json = nlohmann::json;

std::string createSoapRequest(const std::string& wMethod, const std::string& wServiceType, 
    const std::string& wName, const std::string& wState) {

    // Generate new XML document within memory
    pugi::xml_document doc;

    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version")     = "1.0";
    declarationNode.append_attribute("encoding")    = "utf-8";

    auto root = doc.append_child("s:Envelope");
    root.append_attribute("xmlns:s")     = "http://schemas.xmlsoap.org/soap/envelope/";
    root.append_attribute("s:encodingStyle") = "http://schemas.xmlsoap.org/soap/encoding/";

    auto body = root.append_child("s:Body");
    auto setBinarystate = body.append_child((std::string("u:") + wMethod).c_str());
    setBinarystate.append_attribute("xmlns:u") = wServiceType.c_str(); //"urn:Belkin:service:basicevent:1";
        
    pugi::xml_node state;
    if (!wName.empty()) {
        state = setBinarystate.append_child(wName.c_str());
        state.append_child(pugi::node_pcdata).set_value(wState.c_str());
    }

    std::stringstream ss;
    doc.save(ss,"  ");
    //printf("Body: \n%s\n", ss.str().c_str());

    return ss.str();
}

std::string parseSoapResponse(const std::string& response, const std::string& node) {
    // std::string output;

    std::string searchNode = "u:" + node;
    pugi::xml_document doc;
    json j;

    if (doc.load_string(response.c_str()))
    {
        for(auto node : doc.child("s:Envelope").child("s:Body").child(searchNode.c_str()).children())
        {
            // printf("Name: %s Value %s\n", node.name(), node.text().as_string());
            j[node.name()] = node.text().as_string();
        }
    }
    return j.dump();
}
