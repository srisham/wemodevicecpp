#include <curl/curl.h>
#include <iostream>
#include <cstring>

struct response {
  char *memory;
  size_t size;
};

static size_t
mem_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct response *mem = (struct response *)userp;

  char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

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

    CURL *curl;
    CURLcode res;

    std::string url = "http://wemo:49153/upnp/control/basicevent1";

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-type: text/xml; charset=utf-8");
        headers = curl_slist_append(headers, "SOAPACTION: \"urn:Belkin:service:basicevent:1#SetBinaryState\"");
        headers = curl_slist_append(headers, "Connection: keep-alive");
        headers = curl_slist_append(headers, contentLength.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, soap.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); /* redirects! */
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); /* no file */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        else {
        /*
        * After the login POST, we have received the new cookies. Switch
        * over to a GET and ask for the login-protected URL.
        */
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/file");
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L); /* no more POST */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "second curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    std::cout<<std::endl;
  
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