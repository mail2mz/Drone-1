--------------Task1---------- 

uno.ino: code in uno, i2c commmunicate with esp
esp.ino: code in nodemcu, wifi server and i2c communicate with uno

-------------Task2----------

arduino_done_2.0_TWO_axis: code in uno, drone auto balance code form internet

-------------Task3---------- 

drone_uno: code in uno, i2c master to collect data from mpu6050, i2c client to response the command from esp8266
drone_quad_uno: code in uno, based on task2 and drone_uno (i2c client to communicate with esp8266).
drone_muc: code in nodemcu, wifi server for browser, i2c master to relay commend to uno

-------------Schematics-----
drone_control.png

------------Hardware-------
Arduino UNO
NodeMCU esp12e
GY-521 mpu6050
