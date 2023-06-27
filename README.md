# ESP32-DHT11-Ultrasonic
Schematic 
![sch-efisery](https://github.com/Rizky2110/ESP32-DHT11-Ultrasonic/assets/56052061/3ab073bb-4af5-4c4d-a77c-f376b02be0de)

Read data from sensor Temperature and Humidity from DHT11 and distance from the sensor, then store the data in SPIFFS with json form.

-clone repo.
-upload on esp32 devkit.
-use the serial monitor to send commands.

Usage Command in serial monitor :
  -interval --> set the interval time to take data.
  -stream --> change the mode to stream data. 
  -stop --> stop streaming data.
  -read --> read data from datalog.txt in SPIFFS.
  -delete --> delete data datalog.txt in SPIFFS.
