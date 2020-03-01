#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

//const char* ssid     = "5555";      // SSID of local network
//const char* password = "0777898910";   // Password on network
const char* ssid     = "adgn";      // SSID of local network
const char* password = "55555333";   // Password on network

char servername[]= "api.kits4.me";  // remote server we will connect to
char api_path[]="/crcle/dev1.php";
//char servername[]= "gling.co.il";   // remote server we will connect to
//char api_path[]="/D1/dev1.php";

  WiFiClient client;
int server_port=80;//http

void wifiSetup(){
  Serial.println("wifiSetup");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("trying ...");
    delay(100);
  }
  Serial.println("Connected to network");

}

const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }
  return ok;
}
int sendAnsToServer(int user_ans){
  String result;
  int ret=-1;
  String url="GET /tower/getAns.php?a=";
  url.concat(String(user_ans));
  url.concat(" HTTP/1.1");

  Serial.print("url= ");
  Serial.println(url);
  
  Serial.print("connecting to ");
  Serial.println(servername);
  if (client.connect(servername, server_port)) {  //starts client connection, checks for connection
    Serial.println("connected");
  
//   client.println("GET /crcle/dev1.php?p=0 HTTP/1.1"); //Send data
   client.println(url); //Send data
    client.print("Host: ");
    client.println(servername);
    client.println("Connection: close");  //close 1.1 persistent connection  
    client.println(); //end of get request
    skipResponseHeaders();
    
    while (client.connected()) {
      result = client.readStringUntil('\n');
      Serial.print("result=");
      Serial.println(result);
      if (result == "\r") {
        Serial.println("headers received");
        break;
      }
      else {
        ret=result.toInt();
        return ret;
      }
    }
  }
  else {
    Serial.println("sendAnsToServer:: connection failed"); //error message if no client connect
          Serial.print("servername=");
          Serial.print(servername);
          Serial.print("   server_port=");
          Serial.println(server_port);
    Serial.println();
  }
   return ret;
}
bool sendRequest(){
   client.println("GET /crcle/dev1.php HTTP/1.1"); //Send data
    client.print("Host: ");
    client.println(servername);
    client.println("Connection: close");  //close 1.1 persistent connection  
    client.println(); //end of get request
    
    return true;
}
bool dbToNorm(){
  Serial.print("connecting to ");
  Serial.println(servername);
  if (client.connect(servername, server_port)) {  //starts client connection, checks for connection
    Serial.println("connected");
  
   client.println("GET /crcle/dev1.php?p=0 HTTP/1.1"); //Send data
    client.print("Host: ");
    client.println(servername);
    client.println("Connection: close");  //close 1.1 persistent connection  
    client.println(); //end of get request
  }
  else {
    Serial.println("dbToNorm:: connection failed"); //error message if no client connect
          Serial.print("servername=");
          Serial.print(servername);
          Serial.print("   server_port=");
          Serial.println(server_port);
    Serial.println();
//    greenOFF();
  }    return true;
}
String sendDataToServer(){
  String result;
  Serial.print("connecting to ");
  Serial.println(servername);
  if (client.connect(servername, server_port)) {  //starts client connection, checks for connection
    Serial.println("connected");
  
    if(sendRequest() && skipResponseHeaders()) {
        while (client.connected()) {
          result = client.readStringUntil('\n');
          Serial.print("result=");
          Serial.println(result);
          if (result == "\r") {
            Serial.println("headers received");
            break;
          }
          else {
            return result;
          }
        }
//        String line = client.readStringUntil('\n');
//          Serial.println("--------------------");
//          Serial.print("line=");
//          Serial.println(line);
//          Serial.println("--------------------");
    }
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
          Serial.print("servername=");
          Serial.print(servername);
          Serial.print("   server_port=");
          Serial.println(server_port);
    Serial.println();
//    greenOFF();
  }

   return result;
}

int getStatusFromServer(){
  String result;
  int ret=-1;
  Serial.print("connecting to ");
  Serial.println(servername);
  if (client.connect(servername, server_port)) {  //starts client connection, checks for connection
    Serial.println("connected");
  
    if(sendRequest() && skipResponseHeaders()) {
        while (client.connected()) {
          result = client.readStringUntil('\n');
          Serial.print("result=");
          Serial.println(result);
          if (result == "\r") {
            Serial.println("headers received");
            break;
          }
          else {
            ret=result.toInt();
            return ret;
          }
        }
    }
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
          Serial.print("servername=");
          Serial.print(servername);
          Serial.print("   server_port=");
          Serial.println(server_port);
    Serial.println();
//    greenOFF();
  }

   return ret;
}
