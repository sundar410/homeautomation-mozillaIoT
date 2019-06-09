/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Originally based on the HelloServer example
 * Tested on ESP8266, ESP32, Arduino boards with WINC1500 modules (shields or
 * MKR1000)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

//TODO: Hardcode your wifi credentials here (and keep it private)
const char* ssid = "WesternDigital";
const char* password = "asdfghjkl";

/*#if defined(LED_BUILTIN)
//const int ledPin = LED_BUILTIN;
const int ledPin = 13;
const int fanPin = 14;
#else
const int ledPin = 13;  // manually configure LED pin
const int fanPin = 14;
#endif*/

const int ledPin = 13;
const int fanPin = 14;
const int lampPin = 12;

WebThingAdapter* adapter;

const char* ledTypes[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice led("led", "Lamp", ledTypes);
//ThingDevice level("level", "", ledTypes)
ThingProperty ledOn("ON", "", BOOLEAN, "OnOffProperty");
//ThingProperty ledLevel("level", "", NUMBER, "BrightnessProperty");

const char* fanTypes[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice fan("fan", "Fan", fanTypes);
//ThingDevice level("level", "", ledTypes)
ThingProperty fanon("ON", "", BOOLEAN, "OnOffProperty");
//ThingProperty ledLevel("level", "", NUMBER, "BrightnessProperty")

const char* lampTypes[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice lamp("lamp", "Lamp", lampTypes);
//ThingDevice level("level", "", ledTypes)
ThingProperty lampOn("ON", "", BOOLEAN, "OnOffProperty");
//ThingProperty ledLevel("level", "", NUMBER, "BrightnessProperty");

const char* led1Types[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice led1("led", "Lamp", led1Types);
//ThingDevice level("level", "", ledTypes)
ThingProperty ledOn("ON", "", BOOLEAN, "OnOffProperty");
//ThingProperty ledLevel("level", "", NUMBER, "BrightnessProperty");


bool lastOn = false;
bool lastOn1 = false;
bool lastOn2 = false;

void setup(void){
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, HIGH);
  pinMode(lampPin, OUTPUT);
  digitalWrite(lampPin, HIGH);
  pinMode(lampPin, OUTPUT);
  digitalWrite(lampPin, HIGH);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, blink ? LOW : HIGH);
    digitalWrite(fanPin, blink ? LOW : HIGH);// active low led
    digitalWrite(lampPin, blink ? LOW : HIGH);
    blink = !blink;
  }
  digitalWrite(ledPin, HIGH); // active low led
  digitalWrite(fanPin, HIGH);
  digitalWrite(lampPin, HIGH);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  adapter = new WebThingAdapter("w25", WiFi.localIP());

  led.addProperty(&ledOn);
  adapter->addDevice(&led);
  
  
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led.id);
 
  fan.addProperty(&fanon);
  adapter->addDevice(&fan);
  
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(fan.id);
  
  lamp.addProperty(&lampOn);
  adapter->addDevice(&lamp);
  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(lamp.id);
}

void loop(void){
  adapter->update();
  //bool on = ledOn.getValue().boolean;
  //bool on = fanon.getValue().boolean;
  digitalWrite(ledPin, ledOn.getValue().boolean ? LOW : HIGH); // active low led
  digitalWrite(fanPin, fanon.getValue().boolean ? LOW : HIGH); // active low led
  digitalWrite(lampPin, lampOn.getValue().boolean ? LOW : HIGH); // active low led
  if (ledOn.getValue().boolean != lastOn || fanon.getValue().boolean != lastOn1 || lampOn.getValue().boolean != lastOn2 ) {
    if(ledOn.getValue().boolean != lastOn){
      Serial.print(led.id);
      Serial.print(": ");
      Serial.println(ledOn.getValue().boolean);
      }
    if(fanon.getValue().boolean != lastOn1){
       Serial.print(fan.id);
      Serial.print(": ");
      Serial.println(fanon.getValue().boolean);
      }
    if(lampOn.getValue().boolean != lastOn2){
      Serial.print(lamp.id);
      Serial.print(": ");
      Serial.println(lampOn.getValue().boolean);
      } 
  }
  if(ledOn.getValue().boolean != lastOn){lastOn = ledOn.getValue().boolean;}
  if(fanon.getValue().boolean != lastOn1){lastOn1 = fanon.getValue().boolean;}
  if(lampOn.getValue().boolean != lastOn2){lastOn2 = lampOn.getValue().boolean;}
}
