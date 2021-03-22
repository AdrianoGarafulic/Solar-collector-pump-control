/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 



#include <OneWire.h>
#include <DallasTemperature.h>
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
float a=5;
const int oneWireBus = 14;   
byte ledPin = 2;
char ssid[] = "iPhone";           // SSID of your home WiFi
char pass[] = "pagtrg40";            // password of your home WiFi

char auth[] = "JR_bvhOQoTKYY7t2hQRgQXGpCunzzT00";
BlynkTimer timer;

unsigned long askTimer = 0;
float temperatureC;
String mystring;
int s=192;
int b=168;
int c= 1;
int d= 35;
String adresa;
//char b[16]="192.168.331.335";
IPAddress IP = IPAddress(s,b,c,d);
WiFiClient client;

//char IP[16] = "123.234.111.222";


OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);



BLYNK_CONNECTED() {
  // Request the latest state from the server
     Blynk.syncVirtual(V5);
     Blynk.syncVirtual(V6);
     

  // Alternatively, you could override server state using:
  //Blynk.virtualWrite(V2, ledState);
}

 BLYNK_WRITE(V5) {
  
 
   if((param.asInt()==1))
  {
    ESP.reset();
  }

 }
 
 BLYNK_WRITE(V6) {
 
  adresa=param.asStr();
   
  String l = getValue(adresa, '.', 0);
String m = getValue(adresa, '.', 1);
String n = getValue(adresa, '.', 2); 
String k = getValue(adresa, '.', 3);
s=l.toInt();
b=m.toInt();
c=n.toInt();
d=k.toInt();
 IP = IPAddress(s,b,c,d);
 
  
/*   int str_len= IP.length() + 1; 
 char char_array[str_len];
IP.toCharArray(char_array, str_len);
b=char_array;*/
//(b*)IP.c_str();
  }

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void sendSensor()
{


  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4,temperatureC);
//   Blynk.virtualWrite(V7,char_array);
}


void setup() {

  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  pinMode(ledPin, OUTPUT);
  
  sensors.begin();

  Blynk.config(auth);
  Blynk.connect();
 timer.setInterval(100L, sendSensor);
 
}


void loop () {

    sensors.requestTemperatures(); 
 temperatureC = sensors.getTempCByIndex(0);
  
mystring = String(temperatureC);
 //Serial.println(mystring);

Serial.println(IP);

IPAddress server(IP);       // the fix IP address of the server


  client.connect(server, 80);   // Connection to the server
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
 // Serial.println(".");
  client.println(mystring+ "\r");  // sends the message to the server
  //String answer = client.readStringUntil('\r');   // receives the answer from the sever
  //Serial.println("from server: " + answer);
  client.flush();
  digitalWrite(ledPin, HIGH);
  delay(2000);                  // client will trigger the communication after two seconds
  Blynk.run();
  timer.run();
}
