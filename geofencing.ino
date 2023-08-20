//#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
//#include <TinyGPS++.h>

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "geofencing-714a6-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "qqWUOXLUwRa3rPptLMUESCmT5Rvaukgh1ISEIUDO"  
#define WIFI_SSID "SKYNET"
#define WIFI_PASSWORD "xstre@m_03"


const float maxDistance = 100;
float initialLatitude =11.93933448116142;
float initialLongitude =79.83072437475579;

float latitude=11.939654460974737, longitude=79.82957580527884;  
 
void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
   Serial.println("Sending_Data");
   float distance = getDistance(latitude, longitude, initialLatitude, initialLongitude);
  //--------------------------------------------------------------
 // Serial.print("Latitude= "); Serial.println(latitude);
 // Serial.print("Lngitude= "); Serial.println(longitude);
 // Serial.print("initialLatitude= "); Serial.println(initialLatitude);
 // Serial.print("initialLngitude= "); Serial.println(initialLongitude);
  Serial.print("geofence radius= "); Serial.println(maxDistance);
  Serial.print("Distance from home = "); Serial.println(distance);
  delay(1000);
  //--------------------------------------------------------------
   if(distance > maxDistance) {
  Serial.println("subject crossed the geofence radius\n");
  Firebase.setString("status", "1 (Subject is outside the geofence)");
    delay(1000);
  }
  else{
   Serial.println("subject is inside the geofence radius\n");
   Firebase.setString("status", "0  (Subject is inside the geofence)");
  delay(1000);
  }

}
float getDistance(float flat1, float flon1, float flat2, float flon2) {

  // Variables
  float dist_calc=0;
  float dist_calc2=0;
  float diflat=0;
  float diflon=0;

  // Calculations
  diflat  = radians(flat2-flat1);
  flat1 = radians(flat1);
  flat2 = radians(flat2);
  diflon = radians((flon2)-(flon1));

  dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
  dist_calc2 = cos(flat1);
  dist_calc2*=cos(flat2);
  dist_calc2*=sin(diflon/2.0);
  dist_calc2*=sin(diflon/2.0);
  dist_calc +=dist_calc2;

  dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));
  
  dist_calc*=6371000.0; //Converting to meters

  return dist_calc;
}
 
    
