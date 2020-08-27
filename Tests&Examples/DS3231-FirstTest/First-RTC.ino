#include <Wire.h> //I2C library
#include <RtcDS3231.h> //RTC library




  #include <WiFiUdp.h>
  #include <ESP8266WiFi.h>
  #include <NTPClient.h>
  const char* ssid       = "Wlan ohne Elan";
  const char* password   = "***";
  const long utcOffsetInSeconds = 3600;



RtcDS3231<TwoWire> RTC(Wire);


void setup() {

  Serial.begin(9600);

  RTC.Begin();    //Starts I2C

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime()+ 3600 - 8760*3600*30 - 3600*24*7;
  Serial.println(epochTime);
  RTC.SetDateTime(epochTime);

}

void loop() {

  RtcDateTime currentTime = RTC.GetDateTime();    //get the time from the RTC

  char str[20];   //declare a string as an array of chars
  Serial.println(currentTime.Year());
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d allows to print an integer to the string
          currentTime.Year(),   //get year method
          currentTime.Month(),  //get month method
          currentTime.Day(),    //get day method
          currentTime.Hour(),   //get hour method
          currentTime.Minute(), //get minute method
          currentTime.Second()  //get second method
         );

  Serial.println(str); //print the string to the serial port

  delay(2000); //2 seconds delay

}
