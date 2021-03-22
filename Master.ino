/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how to synchronize Button widget
  and physical button state.

  App project setup:
    Button widget attached to V2 (Switch mode)
 *************************************************************/

/* Comment this out to disable prints and save space */
#include <OneWire.h>
#include <DallasTemperature.h>
#define BLYNK_PRINT Serial
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char ssid[] = "denis2";           // SSID of your home WiFi
char pass[] = "10203040";   // password of your home WiFi

char auth[] = "90ab642c1c0242b89e63e7bea550c35a";
// Set your LED and physical button pins here
const int ledPin = 12;//pin releja
byte ledPin1 = 2;
bool stanje=HIGH;//varijabla automatike
bool a=HIGH;
bool stanje2;//za ocitat stanje direkt sa releja
const int oneWireBus = 4;   //senzor temp
String IP;

float temp_cijev;
float temp_bojler;

OneWire oneWire(oneWireBus);


// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

BlynkTimer timer;

WiFiServer server(80);     


//void checkPhysicalButton();
int ledState = LOW;

// Every time we connect to the cloud...
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V3);
   Blynk.syncVirtual(V1);
     Blynk.syncVirtual(V5);
      Blynk.syncVirtual(V6);
     

  // Alternatively, you could override server state using:
  //Blynk.virtualWrite(V2, ledState);
}


// When App button is pushed - switch the state
BLYNK_WRITE(V3) {//botun auto/manual

/*if((param.asInt()==1))
  {
     digitalWrite(ledPin, HIGH);
     a=HIGH;
  }
  else
  {
     digitalWrite(ledPin, LOW);
     a=LOW;
  }*/

  
  ledState = param.asInt();
  //digitalWrite(ledPin, ledState);
  stanje=param.asInt();
   a=param.asInt();
 }
 
 BLYNK_WRITE(V6) {
   IP = param.asInt();
}
 BLYNK_WRITE(V5) {//reset botun
  
 
   if((param.asInt()==1))
  {
    //ESP.reset();
  }

 }

BLYNK_WRITE(V1)
 {
 if((param.asInt()==1)&(stanje==LOW))
  {
     digitalWrite(ledPin, HIGH);
     a=HIGH;
  }
  else
  {
     digitalWrite(ledPin, LOW);
     a=LOW;
  }
 }

  


void sendSensor()
{
   IP= WiFi.localIP().toString();
 // or dht.readTemperature(true) for Fahrenheit

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, temp_cijev);
  Blynk.virtualWrite(V7, stanje2);
  Blynk.virtualWrite(V8, temp_bojler);
  Blynk.virtualWrite(V6, IP);
  Blynk.virtualWrite(V9,  WiFi.RSSI());
}


void setup()
{
  // Debug console
 Serial.begin(115200);       
 
  sensors.begin();
  // only for debug
 //WiFi.config(ip, gateway, subnet);       // forces to use the fix IP ne rodi blynk
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

   server.begin();                         // starts the server
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());
  pinMode(ledPin1, OUTPUT);

   Blynk.config(auth);
  Blynk.connect();
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // Setup a function to be called every 100 ms
   timer.setInterval(1000L, sendSensor);
}

void loop()
{

   stanje2=  digitalRead(ledPin);
   //Serial.println(stanje2);
    
    sensors.requestTemperatures(); 
   temp_bojler= sensors.getTempCByIndex(0);
   
 
  
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(ledPin1, LOW);  // to show the communication only (inverted logic)
      String request = client.readStringUntil('\r');    // receives the message from the client
      temp_cijev=request.toFloat();
      
     // Serial.print("From client: "); Serial.println(temp_cijev);
      client.flush();
      client.println("Hi client! No, I am listening.\r"); // sends the answer to the client
      digitalWrite(ledPin1, HIGH);
    }

    
    client.stop();                // tarminates the connection with the client
  }

  if (((temp_cijev-temp_bojler)>0.5)&&((temp_cijev)&&(temp_bojler)>15)&&((temp_cijev)&&(temp_bojler)<90))
{
       if(stanje==HIGH){//AKO JE NA AUTOMATIKU
  digitalWrite(ledPin, HIGH);
  a=HIGH;
  //  ledState = LOW
  }}
  else 
{      if(stanje==HIGH){
  digitalWrite(ledPin, LOW);
  a=LOW;
  //  ledState = LOW;
       }
    
}
if (stanje==LOW)
       { digitalWrite(ledPin, a);
       }
    
  
  Blynk.run();
  timer.run();
}
