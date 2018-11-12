
#include <Wire.h>

#include <ArduinoJson.h>


//Gyro Variables
float elapsedTime, time, timePrev;        //Variables for time control
int gyro_error=0;                         //We use this variable to only calculate once the gyro data error
float Gyr_rawX, Gyr_rawY, Gyr_rawZ;     //Here we store the raw data read 
float Gyro_angle_x, Gyro_angle_y;         //Here we store the angle value obtained with Gyro data
float Gyro_raw_error_x, Gyro_raw_error_y; //Here we store the initial gyro data error

//Acc Variables
int acc_error=0;                         //We use this variable to only calculate once the Acc data error
float rad_to_deg = 180/3.141592654;      //This value is for pasing from radians to degrees values
float Acc_rawX, Acc_rawY, Acc_rawZ;    //Here we store the raw data read 
float Acc_angle_x, Acc_angle_y;          //Here we store the angle value obtained with Acc data
float Acc_angle_error_x, Acc_angle_error_y; //Here we store the initial Acc data error

float Total_angle_x, Total_angle_y;



void setup() {

 Wire.begin(8);                /* join i2c bus with address 8 */

 Wire.onReceive(receiveEvent); /* register receive event */

 Serial.begin(9600);           /* start serial for debug */

 Wire.beginTransmission(0x68);           //begin, Send the slave adress (in this case 68)              
  Wire.write(0x6B);                       //make the reset (place a 0 into the 6B register)
  Wire.write(0x00);
  Wire.endTransmission(true);             //end the transmission
  
  Wire.beginTransmission(0x68);           //Start communication with the address found during search.
  Wire.write(0x1C);                       //We want to write to the ACCEL_CONFIG register
  Wire.write(0x10);                       //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true); 


  pinMode(3, OUTPUT);

 
   Serial.println("Setup complete\n");;           /* print the request data */
}



void loop() {


 //////////////////////////////////////Acc read/////////////////////////////////////

  Wire.beginTransmission(0x68);     //begin, Send the slave adress (in this case 68) 
  Wire.write(0x3B);                 //Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);      //keep the transmission and next
  Wire.requestFrom(0x68,6,true);    //We ask for next 6 registers starting withj the 3B  
  /*We have asked for the 0x3B register. The IMU will send a brust of register.
  * The amount of register to read is specify in the requestFrom function.
  * In this case we request 6 registers. Each value of acceleration is made out of
  * two 8bits registers, low values and high values. For that we request the 6 of them  
  * and just make then sum of each pair. For that we shift to the left the high values 
  * register (<<) and make an or (|) operation to add the low values.
  If we read the datasheet, for a range of+-8g, we have to divide the raw values by 4096*/    
  Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
  Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
  Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ; 
 /*Now in order to obtain the Acc angles we use euler formula with acceleration values
 after that we substract the error value found before*/  
 /*---X---*/
 Acc_angle_x = (atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_x;
 /*---Y---*/
 Acc_angle_y = (atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_y;    
 
 /*Uncoment the rest of the serial prines
 * I only print the Y raw acceleration value */
 //Serial.print("AccX raw: ");
 //Serial.print(Acc_rawX);
 //Serial.print("   |   ");
 //Serial.print("AccY raw: ");
 //Serial.print(Acc_rawY);
 //Serial.print("   |   ");
 Serial.print("AccZ raw: ");
 Serial.print(Acc_rawZ);
 Serial.print("   |   ");
 //Serial.print("AccX angle: ");
 //Serial.print(Acc_angle_x);
 //Serial.print("   |   ");
 Serial.print("AccY angle: ");
 Serial.print(Acc_angle_y);
 Serial.println(" ");


 delay(400);

}



void processCall(String command){

      DynamicJsonBuffer jsonBuffer;

      JsonObject& root= jsonBuffer.parseObject(command);

      

       if (root.success()) {

          int gpio = atoi(root["gpio"]);

          Serial.println(gpio);

          int state = atoi(root["state"]);

          Serial.println(state);



          //set GPIO state  

          digitalWrite(gpio, state);

       }

}





// function that executes when data is received from master

void receiveEvent(int howMany) {

  String data="";

 while (0 <Wire.available()) {

    char c = Wire.read();      /* receive byte as a character */

    data += c;

    

  }

    Serial.println(data);           /* print the request data */

    processCall(data);         /* to newline */

}
