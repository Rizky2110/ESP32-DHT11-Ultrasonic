#include <Arduino.h>
#include <DHT.h>
#include <Arduino_JSON.h>
#include <hardware_module.h>
#include "time.h"
#include <spiffs_module.h>
#include <string>

#define DHTTYPE DHT11

JSONVar dataSensor;

HARDWARE Hardware;
IFile file;

int intervalTime = 5000; // interval ambil data
int currentTime = 0;
float dataHumidity;
float dataTemperaturC;
float heatIndexC;

bool stream = false;

#define SOUND_SPEED 0.034

long duration;
float distanceCm;

DHT dht(Hardware.pDataDhtSensor, DHTTYPE);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Hardware.setup_hardware();
  dht.begin();
  // Serial.println(F("DHT11 test!"));
  file.setupFile();
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    Serial.print("Command = ");
    Serial.println(command);

    if (command.equals("help") || command.equals("?"))
    {
      Serial.println("List of Commad :");
      Serial.println("Type interval to set data interval");
      Serial.println("Type stream to stream data ");
      Serial.println("Type stop to stop stream data");
      Serial.println("Type read to read data log");
      Serial.println("Type delete to delete data log");
    }
    else if (command.equals("interval"))
    {
      Serial.println("Set your value interval with minimum value 2000 =");
      while (Serial.available() == 0)
      {
      }
      int interval_new = Serial.parseInt();
      if (interval_new < 2000)
      {
        intervalTime = 2000;
      }
      else
        intervalTime = interval_new;

      Serial.print("Your interval time has been set to = ");
      Serial.println(intervalTime);
    }
    else if (command.equals("stream"))
    {
      stream = true;
      Serial.print("Set Stream to ");
      Serial.println(stream);
    }
    else if (command.equals("stop"))
    {
      stream = false;
      Serial.print("Set Stream to ");
      Serial.println(stream);
    }
    else if (command.equals("delete"))
    {
      SPIFFS.remove("/datalog.txt");
      delay(2000);
      ESP.restart();
    }
    else if (command.equals("read"))
    {
      File file2 = SPIFFS.open("/datalog.txt");

      if (!file2)
      {
        Serial.println("Failed to open file for reading");
        return;
      }

      Serial.println("File Content:");

      while (file2.available())
      {
        Serial.write(file2.read());
      }
      file2.close();
    }
    else if ((command != "help") || (command != "?") || (command != "interval") || (command != "stream"))
    {
      Serial.println("Type help or ? to see list of all the available commands.");
    }
  }
  if (stream == false)
  {
    if (millis() - currentTime > intervalTime)
    {
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      currentTime = millis();
      dataHumidity = dht.readHumidity();
      dataTemperaturC = dht.readTemperature();
      if (isnan(dataHumidity) || isnan(dataTemperaturC))
      {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      heatIndexC = dht.computeHeatIndex(dataTemperaturC, dataHumidity, false);
      // Clears the trigPin
      digitalWrite(Hardware.pTrigerUltrasonik, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(Hardware.pTrigerUltrasonik, HIGH);
      delayMicroseconds(10);
      digitalWrite(Hardware.pTrigerUltrasonik, LOW);

      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(Hardware.pEchoUltrasonik, HIGH);

      // Calculate the distance
      distanceCm = duration * SOUND_SPEED / 2;

      // plot data to json
      dataSensor["data Humidity"] = dataHumidity;
      dataSensor["data Temperatur"] = dataTemperaturC;
      dataSensor["data Heat Index"] = heatIndexC;
      dataSensor["data Distance"] = distanceCm;

      delay(100);
      File fileToAppend = SPIFFS.open("/datalog.txt", FILE_APPEND);
      if (!fileToAppend)
      {
        Serial.println("There was an error opening the file for appending");
        return;
      }

      if (fileToAppend.println(dataSensor))
      {
        Serial.println("data was appended to datalog.txt");
      }
      else
      {
        Serial.println("File append failed");
      }
      fileToAppend.close();
    }
  }
  else if (stream == true)
  {
    if (millis() - currentTime > intervalTime)
    {
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      currentTime = millis();
      dataHumidity = dht.readHumidity();
      dataTemperaturC = dht.readTemperature();
      if (isnan(dataHumidity) || isnan(dataTemperaturC))
      {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      heatIndexC = dht.computeHeatIndex(dataTemperaturC, dataHumidity, false);
      // Clears the trigPin
      digitalWrite(Hardware.pTrigerUltrasonik, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(Hardware.pTrigerUltrasonik, HIGH);
      delayMicroseconds(10);
      digitalWrite(Hardware.pTrigerUltrasonik, LOW);

      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(Hardware.pEchoUltrasonik, HIGH);

      // Calculate the distance
      distanceCm = duration * SOUND_SPEED / 2;

      Serial.print("Data Humidity = ");
      Serial.println(dataHumidity);
      Serial.print("Data Temperatur = ");
      Serial.println(dataTemperaturC);
      Serial.print("Data Heat Index = ");
      Serial.println(heatIndexC);
      Serial.print("Data Distance = ");
      Serial.println(distanceCm);

      // plot data to json
      dataSensor["data Humidity"] = dataHumidity;
      dataSensor["data Temperatur"] = dataTemperaturC;
      dataSensor["data Heat Index"] = heatIndexC;
      dataSensor["data Distance"] = distanceCm;

      Serial.print("DATA = ");
      Serial.println(dataSensor);
      delay(100);
      File fileToAppend = SPIFFS.open("/datalog.txt", FILE_APPEND);
      if (!fileToAppend)
      {
        Serial.println("There was an error opening the file for appending");
        return;
      }

      if (fileToAppend.println(dataSensor))
      {
        Serial.println("data was appended to datalog.txt");
      }
      else
      {
        Serial.println("File append failed");
      }
      fileToAppend.close();
    }
  }
}
