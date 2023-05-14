#include <iostream>
#include <cstring>
#include <sstream>
#include <pugixml.hpp>
#include "HttpRequest.h"

std::string createSoapRequest(const std::string& wMethod, const std::string& wServiceType, 
    const std::string& wName = "", const std::string& wState = "") {

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
    std::string output;

    std::string searchNode = "u:" + node;
    pugi::xml_document doc;
    printf("\n");   
    if (doc.load_string(response.c_str()))
    {
        for(auto node : doc.child("s:Envelope").child("s:Body").child(searchNode.c_str()).children())
        {
            printf("name %s value %s\n", node.name(), node.text().as_string());
        }
    }
    printf("\n");
    return output;
}

std::string getBasicEvents(const std::string& wMethod) {
    
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


std::string setBinaryEvents(const std::string& wMethod, 
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

int main(int argc, char** argv) {

    printf("main +\n");
    
    char* state = argv[1];
    std::cout<<"Command: "<<state<<std::endl<<std::endl;

    
    parseSoapResponse(getBasicEvents("GetFriendlyName"), "GetFriendlyNameResponse");
    parseSoapResponse(getBasicEvents("GetMacAddr"), "GetMacAddrResponse");
    parseSoapResponse(getBasicEvents("GetHomeId"), "GetHomeIdResponse");
    parseSoapResponse(getBasicEvents("GetServerEnvironment"), "GetServerEnvironmentResponse");
    parseSoapResponse(getBasicEvents("GetBinaryState"), "GetBinaryStateResponse");

    setBinaryEvents("SetBinaryState", "BinaryState", "1");
    setBinaryEvents("SetBinaryState", "BinaryState", "0");
      
    return 0;
}
