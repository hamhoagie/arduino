#include <SPI.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Cosm.h>
#include <cozir.h>
#include <SoftwareSerial.h>

char ssid[] = "W-K"; //  your network SSID (name) 
char pass[] = "slimemold";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Your Cosm key to let you upload data
char cosmKey[] = "DxGPrqKCBHB7vbT2sKcqpQZcn6GSAKxDcnJraVNMZXJEbz0g";

// setup softwareserial for rx,tx monitoring
SoftwareSerial nss(3,2);
COZIR czr(nss);

// Define the strings for our datastream IDs
char sensorId[] = "C02_5SW";
CosmDatastream datastreams[] = {
  CosmDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
CosmFeed feed(99993, datastreams, 1 /* number of datastreams */);

WiFiClient client;
CosmClient cosmclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("DEBUG - SoftwareSerial");
  Serial.println();
  
  czr.SetOperatingMode(CZR_POLLING);
  Serial.println("DEBUG - CZR POLLING MODE");
  Serial.println();
    
  Serial.println("Starting single datastream upload to Cosm...");
  Serial.println();

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {

  int sensorValue = czr.CO2();
  
  Serial.print("CO2 : ");Serial.println(sensorValue);
  
  datastreams[0].setFloat(sensorValue);


  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());

  Serial.println("Uploading it to Cosm");
  int ret = cosmclient.put(feed, cosmKey);
  Serial.print("cosmclient.put returned ");
  Serial.println(ret);

  Serial.println();
  delay(60000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

