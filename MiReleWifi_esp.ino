/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/Relay01=0 will set the GPIO3 low,
 *    http://server_ip/Relay01=1 will set the GPIO3 high
 
 *    http://server_ip/Relay02=0 will set the GPIO4 low,
 *    http://server_ip/Relay02=1 will set the GPIO4 high
 
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>


const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  

  // prepare GPIO3
  pinMode(3, OUTPUT);
  digitalWrite(3, 1);
  
  // prepare GPIO4
  pinMode(4, OUTPUT);
  digitalWrite(4, 1);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Conecting to  ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conected");
  
  // Start the server
  server.begin();
  Serial.println("Web server On");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {


  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("New Client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val01;
  int val02;
  
  if (req.indexOf("/Relay01=1") != -1)
    val01 = 0;
  else if (req.indexOf("/Relay01=0") != -1)
    val01 = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  
  if (req.indexOf("/Relay02=1") != -1)
    val02 = 0;
  else if (req.indexOf("/Relay02=0") != -1)
    val02 = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO3 according to the request
  digitalWrite(3, val01);
  
  // Set GPIO4 according to the request
  digitalWrite(4, val02);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nRelay01 ";
  s += (val01)?"Activated":"Disactivated" ;
  s += "\r\nRelay02 " ;
  s += (val02)?"Activated":"Disactivated" ;
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
