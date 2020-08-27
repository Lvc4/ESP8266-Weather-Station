//-----------------------------------------------
// Author: Luca Großhennig
// Email: luca.grosshennig@gmx.de
// Publish date: 28-JUL-2020
// Description: This code for demonstration send data from ESP8266 into Google Spreadsheet, deppsleeps
//              and saves data localy if no wifi is available
// update ssid, password and GAS_ID
//-----------------------------------------------
#include <ESP8266WiFi.h>
#include "SparkFunBME280.h"
#include <WiFiClientSecure.h>
#include <SPI.h>
#include "FS.h"

#define durationSleep 900   // in seconds

BME280 BME;
WiFiClientSecure httpsClient;
File myfile;

const char* ssid = "Wlan ohne Elan";
const char* password = "***";
const char* host = "script.google.com";
const int httpsPort = 443;

String filename = "/data.csv";
String GAS_ID = "***";
float t;
float h;
float p;
int retry = 0;

void getSensor() {
  h = BME.readFloatHumidity();
  t = BME.readTempC();
  p = BME.readFloatPressure() / 100;

  if (isnan(h) || isnan(t) || isnan(p)) {

    return;
  }
}
void setup() {
  SPI.begin();
  setup_bme();
  retry = 0;
  getSensor();
  retry = 0;
  bool wifi = setup_wifi();
  retry = 0;
  bool InitOK = InitalizeFileSystem();
  retry = 0;
  if (wifi) {
    if (InitOK) {
      sendSavedData();
      retry = 0;
    }
    retry = 0;
    sendData(t, h, p);
    retry = 0;
  } else {
    if (InitOK) {
      saveData(t, h, p);
      retry = 0;
    }
  }
  ESP.deepSleep(durationSleep * 1e6);
}

void loop() {
}

void setup_bme() {
  BME.setI2CAddress(0x76);
  while ((!BME.beginI2C()) && retry < 10) {
    BME.setI2CAddress(0x76);
    retry++;
    delay(500);
  }
  if (retry == 10) {
    ESP.deepSleep(durationSleep * 1e6);
  }
  retry = 0;
}

boolean setup_wifi() {
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) && retry < 15) {
    delay(500);
    retry++;      // Wenn nach 10 Versuchen nicht mit WiFi verbunden werden kann, deep-sleep
  }
  if (retry == 15 ) {
    return (false);
  }
  return (true);
}

void sendSavedData() {
  if (SPIFFS.exists(filename)) {
    myfile = SPIFFS.open(filename, "r");
    String content;
    while (myfile.position() < myfile.size())
    {
      content = myfile.readStringUntil('\n');
    }
    myfile.close();
  }
  else {
    return;
  }
}

void saveData(float t, float h, float p) {
  if (!SPIFFS.exists(filename)) {
    myfile = SPIFFS.open(filename, "w");
  } else {
    myfile = SPIFFS.open(filename, "a");
  }
  if (!myfile) {
  } else {
    myfile.println(String(millis()) + "; " + String(t) + "; " + String(h) + "; " + String(p) + ";");
    myfile.close();
  }
}

// Function for Send data into Google Spreadsheet
void sendData(float t, float h, float p) {
  httpsClient.setInsecure();

  String url = "https://" + String(host) + "/macros/s/" + GAS_ID + "/exec?temp=" + String(t) + "&hum=" + String(h) + "&press=" + String(p);

  while ((!httpsClient.connect(host, httpsPort)) && (retry < 10)) {
    delay(100);
    retry++;
  }
  if (retry == 10) {
    saveData(t, h, p);
    ESP.deepSleep(durationSleep * 1e6);
  } else {
  }
  httpsClient.print(String("GET ") + url +
                    " HTTP/1.1\r\n" +
                    "Host: " + host +
                    "\r\n" + "Connection: close\r\n\r\n");
}

boolean InitalizeFileSystem() {
  bool initok = false;
  initok = SPIFFS.begin();
  while ((!initok) && retry < 10) {
    SPIFFS.format();
    initok = SPIFFS.begin();
    retry++;
  }
  return initok;
}
