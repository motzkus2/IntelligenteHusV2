#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <MQTT.h>
#include <Servo.h>
#include <DHT_U.h>
#include <Wire.h>


#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;    

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient net;
MQTTClient client;
MQTTClient tsclient;

Servo myservo;

int led =  LED_BUILTIN;
int x = 0;

String data;

// ThingSpeak Settings
char mqttUserName[] = "MrBoard";  // Use any name.
char mqttPass[] = "HUWMVTYT95HSMDV9";      // Change to your MQTT API Key from Account > MyProfile.   
char writeAPIKey[] = "CHFE78WVFKKUPKKA";    // Change to your channel write API key.
long channelID = 1372997;                    // Change to your channel ID.

void receiveEvent(int howMany)
{
  data = "";
      while(1 < Wire.available()) // loop through all but the last
  {
    
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    data += c;
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer

}

char server[] = "mqtt.thingspeak.com";


unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }


  // Serial.print("\nconnecting...");
  // while (!client.connect("YeetPlatform", "yeetinstance", "1432")) {
  //   Serial.print(".");
  //   delay(1000);
  // }

  Serial.print("\nconnecting to thingspeak...");
  while (!tsclient.connect("MkrWifi1010", "thingspeakpub", "HUWMVTYT95HSMDV9")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

 //client.subscribe("yeet/servo");
 //client.subscribe("yeet/led");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if(topic == "yeet/servo")
  {
      Serial.println(myservo.read());
      myservo.write(payload.toInt());
      String status = String(myservo.read());
      client.publish("yeet/servo/status", status);
  }


  if(topic == "yeet/led")
  {
      switch (payload.toInt())
      {
      case 1:
          digitalWrite(led, HIGH);
          break;
      case 0:
          digitalWrite(led, LOW);
          break;
      
      default:
          break;
      }
      
  }

  
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {


  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.


  client.begin("yeetinstance.cloud.shiftr.io", 1883, net);
  client.onMessage(messageReceived);

  tsclient.begin("mqtt.thingspeak.com", 1883, net);
  tsclient.onMessage(messageReceived);
  
  dht.begin();
  myservo.attach(9);

  pinMode(led, OUTPUT);

  Wire.begin(4); 

  delay(1000);
  
  connect();

  Wire.onReceive(receiveEvent);

}

void loop() {
  tsclient.loop();

  if (!tsclient.connected()) {
    connect();
  }
  // publish a message roughly every 10 second.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();

    //client.publish("yeet/dht11/temp", (String)dht.readTemperature(), false, 2);
    //client.publish("yeet/dht11/humidity", (String)dht.readHumidity(), false, 2);

    String topic = ("channels/1372997/publish/CHFE78WVFKKUPKKA");

    tsclient.publish(topic, data);

  }
   delay(100);
  

  
}

void printWiFiStatus() {

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
}
