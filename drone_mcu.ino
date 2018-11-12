
#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>

#include <WiFiUdp.h>

#include <functional>

#include <Wire.h>

WiFiServer server(80); //Initialize the server on Port 80

// prototypes

boolean connectWifi();

void officeLightsOn();

void officeLightsOff();

void kitchenLightsOn();

void kitchenLightsOff();



// Change this before you flash

//#######################################

const char* ssid     = "Hello_IoT"; //enter your ssid/ wi-fi(case sensitive) router name - 2.4 Ghz only

const char* password = "12345678";     // enter ssid password (case sensitive)


const int relayPin1 = 16;

const int relayPin2 = 4;



//#######################################

boolean wifiConnected = false;


void setup()

{


WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); 
Serial.begin(9600);
server.begin(); // Start the HTTP Server 
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
Serial.print("Server IP is: "); // Print the IP to the monitor window 
Serial.println(HTTPS_ServerIP);


  

   pinMode(relayPin1, OUTPUT);

   pinMode(relayPin2, OUTPUT);
   digitalWrite(relayPin1, LOW); //Initial state is ON
    digitalWrite(relayPin2, LOW); //Initial state is ON


  
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
Serial.println("Complete setup");
}

 

void loop()

{


WiFiClient client = server.available();
if (!client) { 
return; 
} 

String request = client.readString();

Serial.println("Somebody has connected :)");

if (request.indexOf("/OFF") != -1){ 
kitchenLightsOff(); 
Serial.println("kitchenLights Off");
}
else if (request.indexOf("/ON") != -1){ 
kitchenLightsOn(); 
Serial.println("kitchenLights ON");
}

// Prepare the HTML document to respond and add buttons:
String s = "HTTP/1.1 200 OK\r\n";
s += "Content-Type: text/html\r\n\r\n";
s += "<!DOCTYPE HTML>\r\n<html>\r\n";
s += "<br><input type=\"button\" name=\"b1\" value=\"Turn LED ON\" onclick=\"location.href='/ON'\">";
s += "<br><br><br>";
s += "<input type=\"button\" name=\"bi\" value=\"Turn LED OFF\" onclick=\"location.href='/OFF'\">";
s += "</html>\n";
//Serve the HTML document to the browser.
client.flush ();
//clear previous info in the stream
client.print (s); // Send the response to the client
delay(1);
Serial.println("Client disonnected" );

//kitchenLightsOn();
//   delay(3000);
//kitchenLightsOff();
//    delay(3000);
//serial.println("kitchenLightsOn/Off");

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//        NodeMcu Switch on / off

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void officeLightsOn() {

    Serial.print("Switch 1 turn on ...");

    digitalWrite(relayPin1, HIGH);

}



void officeLightsOff() {

    Serial.print("Switch 1 turn off ...");

    digitalWrite(relayPin1, LOW);

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//      Arduino Uno Switch on / off

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void kitchenLightsOn() {

    Serial.print("Switch 2 turn on ...");

      Wire.beginTransmission(8); /* begin with device address 8 */

      Wire.write("{\"gpio\":3,\"state\":1}");  /* sends hello string */

      Wire.endTransmission();    /* stop transmitting */

}



void kitchenLightsOff() {

  Serial.print("Switch 2 turn off ...");

    Wire.beginTransmission(8); /* begin with device address 8 */

    Wire.write("{\"gpio\":3,\"state\":0}");  /* sends hello string */

    Wire.endTransmission();    /* stop transmitting */

}
