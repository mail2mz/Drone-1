
#include <Wire.h>

#include <ArduinoJson.h>



void setup() {

 Wire.begin(8);                /* join i2c bus with address 8 */

 Wire.onReceive(receiveEvent); /* register receive event, call receivedEvent when receiving message for i2c address 8 */

 Serial.begin(9600);           /* start serial for debug */

 pinMode(3, OUTPUT);           /* Turn LED on */

 Serial.println("Setup complete");           /* print the request data */
}



void loop() {

 delay(100);

}



void processCall(String command){

      DynamicJsonBuffer jsonBuffer;

      JsonObject& root= jsonBuffer.parseObject(command);

      
 Serial.println("Entering processCall....\n"); 
       if (root.success()) {

          int gpio = atoi(root["gpio"]); /* receive message started with gpio */

          Serial.println(gpio);  /* Display message on the debug port */

          int state = atoi(root["state"]); /* receive message started with state */

          Serial.println(state); /* Display message on the debug port */



          //set GPIO state  

          digitalWrite(gpio, state);  /* Set GPIO state, should be replace by prone control procedure */

       }

}





// function that executes when data is received from master

void receiveEvent(int howMany) {

  String data="";

 Serial.println("Entering receiveEvent....\n"); 
 while (0 <Wire.available()) {

    char c = Wire.read();      /* receive byte as a character */

    data += c;

    

  }

    Serial.println(data);           /* print the request data */

    processCall(data);         /* to newline */

}
