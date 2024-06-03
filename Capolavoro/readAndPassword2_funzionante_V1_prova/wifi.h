#include<WiFiS3.h>

#define PORT 33333

//per wifi
char ssid[] = "Cianuro";
char pass[] = "coronavirus";
int keyIndex = 0;

int status = WL_IDLE_STATUS;
WiFiServer server(PORT);

//stato del wifi (setup)
void printWifiStatus(){
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}



//setup
void setup_wifi(){

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   


    status = WiFi.begin(ssid, pass);
 
    delay(10000);
  }
  server.begin();                          
  printWifiStatus();
}