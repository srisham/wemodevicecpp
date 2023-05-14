#include <iostream>
#include <cstring>
#include "HttpRequest.h"


int main(int argc, char** argv) {

    if(argc < 1) {
        std::cout<< "Usage: ./wemo_connect [COMMAND on/off]"<<std::endl;
        exit(1);
    }
    char* state = argv[1];

    std::cout<<"Command: "<<state<<std::endl<<std::endl;

    std::string soap;
    soap = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
    soap += "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"";
    soap += "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">";
    soap += "<s:Body>";
    soap += "<u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\">";
    soap += "<BinaryState>" + strcmp(state, "on") == 0 ? std::to_string(1) : std::to_string(0) + "</BinaryState></u:SetBinaryState>";
    soap += "</s:Body></s:Envelope>";

    std::string contentLength = "Content-Length: " + std::to_string(soap.length());

    std::string url = "http://wemo:49153/upnp/control/basicevent1";
    
    HttpRequest http;
    http.setUrl(url);
    http.setHeaders("Content-type", "text/xml; charset=utf-8");
    http.setHeaders("SOAPACTION", "\"urn:Belkin:service:basicevent:1#SetBinaryState\"");
    http.setHeaders("Connection", "keep-alive");
    http.setHeaders("Content-Length", soap);

    std::string response = http.sendRequest();

    // headers = curl_slist_append(headers, "Content-type: text/xml; charset=utf-8");
    // headers = curl_slist_append(headers, "SOAPACTION: \"urn:Belkin:service:basicevent:1#SetBinaryState\"");
    // headers = curl_slist_append(headers, "Connection: keep-alive");
    // headers = curl_slist_append(headers, contentLength.c_str());
    
    std::cout << response.c_str() << std::endl;
  
    return 0;
}

// WiFiClient netSocket;               // network socket to device
// const char wemo[] = "192.168.0.13"; // device address
// int port = 49153;                   // port number
// String route = "/upnp/control/basicevent1";  // API route
// String soap;                        // string for the SOAP request
// boolean wemoState = 1;              // whether the WeMo is on or off

// void setup() {
//   Serial.begin(9600);               // initialize serial communication
//   // while you're not connected to a WiFi AP,
//   while ( WiFi.status() != WL_CONNECTED) {
//     Serial.print("Attempting to connect to Network named: ");
//     Serial.println(SECRET_SSID);
//     WiFi.begin(SECRET_SSID, SECRET_PASS);     // try to connect
//     delay(2000);
//   }

//   // When you're connected, print out the device's network status:
//   IPAddress ip = WiFi.localIP();
//   Serial.print("IP Address: ");
//   Serial.println(ip);
//   // set up the SOAP request string. This formatting is just
//   // for readability of the code:
//   soap = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
//   soap += "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"";
//   soap += "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">";
//   soap += "<s:Body>";
//   soap += "<u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\">";
//   soap += "<BinaryState>1</BinaryState></u:SetBinaryState>";
//   soap += "</s:Body></s:Envelope>";
// }

// void loop() {
//   HttpClient http(netSocket, wemo, port); // make an HTTP client
//   http.connectionKeepAlive();             // keep the connection alive
//   http.beginRequest();                    // start assembling the request
//   http.post(route);                       // set the route
//   // add the headers:
//   http.sendHeader("Content-type", "text/xml; charset=utf-8");
//   String soapAction = "\"urn:Belkin:service:basicevent:1#SetBinaryState\"";
//   http.sendHeader("SOAPACTION", soapAction);
//   http.sendHeader("Connection: keep-alive");
//   http.sendHeader("Content-Length", soap.length());
//   http.endRequest();                      // end the request
//   http.println(soap);                     // add the body
//   Serial.println("request sent");

//   while (http.connected()) {       // while connected to the server,
//     if (http.available()) {        // if there is a response from the server,
//       String result = http.readString();  // read it
//       Serial.print(result);               // and print it
//     }
//   }
//   Serial.println();             // end of the response
//   if (wemoState == 1) {         // if the wemo's on
//     soap.replace(">1<", ">0<"); // turn it off next time
//   } else {                      // otherwise
//     soap.replace(">0<", ">1<"); // turn it on next time
//   }
//   wemoState = !wemoState;       // toggle wemoState
//   delay(5000);                  // wait 5 seconds
// }