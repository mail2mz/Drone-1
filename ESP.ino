
#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>

#include <WiFiUdp.h>

#include <functional>

#include <Wire.h>

WiFiServer server(80); //Initialize the server on Port 80

boolean connectWifi();

void droneUp();

void droneDown();



// Change this before you flash

//#######################################

const char* ssid     = "Hello_IoT"; //enter your ssid/ wi-fi(case sensitive) router name - 2.4 Ghz only

const char* password = "12345678";     // enter ssid password (case sensitive)


const int relayPin1 = 16;


//#######################################

boolean wifiConnected = false;


void setup()

{

//Setup WIFI mode, ssid, password
WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); 

//Setup debug port boundrate
Serial.begin(9600);

//Setup http server
server.begin(); // Start the HTTP Server
 
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
Serial.print("Server IP is: "); // Print the IP to the monitor window 
Serial.println(HTTPS_ServerIP);
  
//Setup GPIO pin as output
pinMode(relayPin1, OUTPUT);
digitalWrite(relayPin1, LOW); //Initial state is ON



 //Setup i2c client 
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

    if (request.indexOf("/Down") != -1){ 
            droneDown(); 
            Serial.println("droneDown command received via wifi");
    }
    else if (request.indexOf("/Up") != -1){ 
            droneUp(); 
            Serial.println("droneUp command received via wifi");
    }

// Prepare the HTML document to respond and add buttons:
String s = "HTTP/1.1 200 OK\r\n";
s += "Content-Type: text/html\r\n\r\n";
s += "<!DOCTYPE HTML>\r\n<html>\r\n";
s += "<br><input type=\"button\" name=\"b1\" value=\"Drone Up\" onclick=\"location.href='/Up'\">";
s += "<br><br><br>";
s += "<input type=\"button\" name=\"bi\" value=\"Drone Down\" onclick=\"location.href='/Down'\">";
s += "</html>\n";
//Serve the HTML document to the browser.
client.flush ();
//clear previous info in the stream
client.print (s); // Send the response to the client
delay(1);
Serial.println("Client disonnected" );
delay(500);
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//      Notify Arduino Uno to control prone

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void droneUp() {

      Serial.print("Please ask drone to fly up ...");

      Wire.beginTransmission(8); /* begin with device address 8 */

      Wire.write("{\"gpio\":3,\"state\":1}");  /* sends hello string */

      Wire.endTransmission();    /* stop transmitting */

}

void droneDown() {

      Serial.print("Please ask drone to fly down  ...");

      Wire.beginTransmission(8); /* begin with device address 8 */

      Wire.write("{\"gpio\":3,\"state\":0}");  /* sends hello string */

      Wire.endTransmission();    /* stop transmitting */

}
