#include "ESP8266.h"
#include <SoftwareSerial.h>

const char* SSID = "YOUR SSID";
const char* PASSWORD = "YOUR PASSWORD";

int PinRele01 = 13;
int PinRele02 = 3;
int PinRele03 = 4;
int PinRele04 = 5;


SoftwareSerial softSerial(10, 11); // RX, TX
ESP8266 wifi(softSerial);

void setup(void)
{
  pinMode(PinRelay01, OUTPUT);
  pinMode(PinRelay02, OUTPUT);
  pinMode(PinRelay03, OUTPUT);
  pinMode(PinRelay04, OUTPUT);

  Serial.begin(9600);
  Serial.print("Begin with the configuration\r\n");

  wifi.restart();
  delay(500);
  if (wifi.setOprToStationSoftAP()) {
    Serial.print("AP y STA mode ok\r\n");
  }
  else {
    Serial.print("AP y STA mode Error\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Conected to AP \r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  }
  else {
    Serial.print("Error to connect AP\r\n");
  }

  if (wifi.enableMUX()) {
    Serial.print("Multiple conection ok\r\n");
  }
  else {
    Serial.print("Multiple conection Error\r\n");
  }

  if (wifi.startTCPServer(80)) {
    Serial.print("Star web server ok\r\n");
  }
  else {
    Serial.print("Star web server Error\r\n");
  }

  if (wifi.setTCPServerTimeout(1)) {
    Serial.print("Time to disconect to server 1 seg  ok\r\n");
  }
  else {
    Serial.print("Time to disconect to server 1 seg Error\r\n");
  }

  Serial.println("Config finish \r\n");
}

#define wifiWrite(A) wifi.send(mux_id, (uint8_t*) A, sizeof(A) - 1);
void loop(void)
{
  uint8_t buffer[128] = { 0 };
  uint8_t mux_id;

  uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
  if (len > 0) {
    Serial.print("Recibido de: ");
    Serial.print(mux_id);
    Serial.print("\r\n");

    wifiWrite("HTTP/1.1 200 OK\r\nnContent-Type: /html\r\nConnection: close\r\n\r\n");
    wifiWrite("<html>\n<head>\n<title>MyRelayWifi</title>\n</head>\n<body>");
    wifiWrite("</body></html>");
  
    for (uint32_t i = 0; i < len; i++) {
      char c = (char)buffer[i];
      if (c == '?')
      {
        if ((char)buffer[i + 6] == '1') // You can change the control character in the settings App.
        {
          digitalWrite(PinRelay01, HIGH);
          Serial.println("Relay01 ON");
        }

       if ((char)buffer[i + 6] == '2')
        {
          digitalWrite(PinRelay01, LOW);
          Serial.println("Relay01 OFF");
        }

        if ((char)buffer[i + 6] == '3')
        {
          digitalWrite(PinRelay02, HIGH);
          Serial.println("Relay02 ON");
        }
        if ((char)buffer[i + 6] == '4')
        {
          digitalWrite(PinRelay02, LOW);
          Serial.println("Relay02 OFF");
        }
        if ((char)buffer[i + 6] == '5')
        {
          digitalWrite(PinRelay03, HIGH);
          Serial.println("Relay03 ON");
        }
        if ((char)buffer[i + 6] == '6')
        {
          digitalWrite(PinRelay03, LOW);
          Serial.println("Relay03 OFF");
        }
        if ((char)buffer[i + 6] == '7')
        {
          digitalWrite(PinRelay04, HIGH);
          Serial.println("Relay04 ON");
        }
        if ((char)buffer[i + 6] == '8')
        {
          digitalWrite(PinRelay04, LOW);
          Serial.println("Relay04 OFF");
        }
        
        break;
      }
    }
  }
} 
