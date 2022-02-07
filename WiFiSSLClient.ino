
/*
T3.   ahis example creates a client object that connects and transfers
data using always SSL.

It is compatible with the methods normally related to plain
connections, like client.connect(host, port).

Written by Arturo 
last revision November 2015

*/

#include <SPI.h>
#include <WiFiNINA.h>
#include "Arduino_SensorKit.h"

#include "arduino_secrets.h" 

#define SensorPin A3          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

int LED = 6;

int R1= 1000;
int Ra=25;

int ECPin= A1;
int ECGround=A2;
int ECPower =A4;

//float raw= 0;

float Temperature=10;
float EC=0;
float EC25 =0;
int ppm =0;
 
 
float raw= 0;
float Vin= 5;
float Vdrop= 0;
float Rc= 0;
float buffer=0;

float K=2.88;

const int analogOutPin = 9; // Analog output pin that the LED is attached to


int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "script.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

void setup() {

  
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("V0.1");

  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);

  WiFi.setTimeout(120 * 1000);

  pinMode(ECPin,INPUT);
  pinMode(ECPower,OUTPUT);//Setting pin for sourcing current
  pinMode(ECGround,OUTPUT);//setting pin for sinking current
  digitalWrite(ECGround,LOW);//We can leave the ground connected permanantly
 



  Environment.begin();

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

//  connectWiFi();
  
  Serial.println("Connected to WiFi");
  digitalWrite(LED, HIGH);
  printWiFiStatus();
//  serverSend();

//  float temp = Environment.readTemperature();
//  float humidity = Environment.readHumidity();
//  int light = analogRead(A3);
//  
//  Serial.print(light);
//
//  Serial.println("\nStarting connection to server...");
//  // if you get a connection, report back via serial:
//  if (client.connect(server, 443)) {
//    Serial.println("connected to server");
//    // Make a HTTP request:
//    client.println("GET https://script.google.com/macros/s/AKfycbzVa3Km3y9gHxXs_EHhtpCRdEy5xKQzDvR4-iy_B4v7CunX3PeuuhSg0EGYEzc9XxO25g/exec?Temp=" + String(temp) + "&Humidity=" + String(humidity) + "&Light=" + String(light));
//    client.println("Host: script.google.com");
//    client.println("Connection: close");
//    client.println();
  
}

void(* resetFunc) (void) = 0;//declare reset function at address 0

void loop() {

//  int light = analogRead(A0);
  
  // if there are incoming bytes available
  // from the server, read them and print them:
//  while (client.available()) {
//    char c = client.read();
//    Serial.write(c);
//  }
//
//  if (status != WL_CONNECTED) {
//    connectWiFi();  }  

  digitalWrite(ECPower,HIGH);
  raw= analogRead(ECPin);
  raw= analogRead(ECPin);// This is not a mistake, First reading will be low beause if charged a capacitor
  digitalWrite(ECPower,LOW);

  R1=(R1+Ra);// Taking into acount Powering Pin Resitance

  Serial.print("raw: ");
  Serial.println(raw);

  Serial.print("pH");
  Serial.println(analogRead(SensorPin));

//  Vdrop= (Vin*raw);
//  
//  Serial.print("Vdrop: ");
//  Serial.println(Vdrop);
//  
//  Rc=(Vdrop*R1)/(Vin-Vdrop);
//
//  Serial.print("Rc: ");
//  Serial.println(Rc);
//  
//  Rc=Rc-Ra; //acounting for Digital Pin Resitance
//
//  Serial.print("Rc: ");
//  Serial.println(Rc);
//  
//  EC = 1000/(Rc*K);
//
//  Serial.print("EC: ");
//  Serial.println(EC);
    
//  serverSend();

  delay(10000);

  resetFunc();

  // if the server's disconnected, stop the client:
//  if (!client.connected()) {
//    Serial.println();
//    Serial.println("disconnecting from server.");
//    client.stop();
//
    // do nothing forevermore:
//    while (true);
//  }
}

void serverSend() {

//  float temp = Environment.readTemperature();
//  float humidity = Environment.readHumidity();
  int light = analogRead(A0);
  
//  Serial.print(light);

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
//    client.println("GET https://script.google.com/macros/s/AKfycbzVa3Km3y9gHxXs_EHhtpCRdEy5xKQzDvR4-iy_B4v7CunX3PeuuhSg0EGYEzc9XxO25g/exec?Temp=" + String(temp) + "&Humidity=" + String(humidity) + "&Light=" + String(light));
    
    client.println("GET https://script.google.com/macros/s/AKfycbzVa3Km3y9gHxXs_EHhtpCRdEy5xKQzDvR4-iy_B4v7CunX3PeuuhSg0EGYEzc9XxO25g/exec?Temp=" + String(light));

    client.println("Host: script.google.com");
    client.println("Connection: close");
    client.println();
    Serial.println("disconnecting from server.");
//    client.stop();

    blinkLED(500, 500);
    
  }

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

void connectWiFi() {
  
  // attempt to connect to WiFi network:
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  status = WiFi.begin(ssid, pass);

  // wait 10 seconds for connection:
  delay(10000);
  

  blinkLED(1000,500);
  
}
void blinkLED(int low_delay, int high_delay) {
    digitalWrite(LED, LOW);
    delay(low_delay);
    digitalWrite(LED, HIGH);
    delay(high_delay);
    digitalWrite(LED, LOW);
    delay(low_delay);
    digitalWrite(LED, HIGH);
    delay(high_delay);
    digitalWrite(LED, LOW);
    delay(low_delay);
    digitalWrite(LED, HIGH);

}
