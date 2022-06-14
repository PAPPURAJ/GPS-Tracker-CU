#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "gps-tracker-4659a-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "tzkjNhRT2mTbGIlyJWDwNRAs8c64D9b1iPzjg7R7"
#define WIFI_SSID "GPS-CU"
#define WIFI_PASSWORD "55555555"

#include <TinyGPS++.h>

FirebaseData firebaseData;
FirebaseJson json;


double Lat=10,Lon=11;
TinyGPSPlus gps;



void writeDB(String field,String value){
 Firebase.setString(firebaseData, "/Location/"+field,value );
  
}







void setup() {

  
  pinMode(D4, OUTPUT);
  
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(D4, 1);
    delay(100);
    Serial.print(".");
    digitalWrite(D4, 0);
    delay(100);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  writeDB("Test","Ok");

}

bool led=true;

void loop() {
  
   while(Serial.available())              //While there are incoming characters  from the GPS
  {
    gps.encode(Serial.read());           //This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())          //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
   
    Lat=gps.location.lat();
    Lon=gps.location.lng();
    Serial.println(Lat,6);
    Serial.println(Lon,6);

    led=!led;
    digitalWrite(D4,led);

    writeDB("Lat",String(Lat,6));
    writeDB("Lon",String(Lon,6));

  }

}
