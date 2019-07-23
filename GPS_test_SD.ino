#include <Key.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <LCD.h>
#include <SoftwareSerial.h>
#include "Adafruit_GPS.h"
#include "Adafruit_BMP085.h"
#include <SD.h>
#include <SPI.h>

SoftwareSerial mySerial(3, 2); // Initializing the software serial port.
Adafruit_GPS GPS(&mySerial);

String NMEA1; //Variable for first NMEA sentence
String NMEA2; //Variable for second NMEA sentence
char c; //To read character objects from GPS.

int chipSelect = 4; // chipSelect pin for the SD card Reader.
File mySensorData; // Data object you will write your sensor data.

void setup()
{
 Serial.begin(115200);
 GPS.begin(9600);
 GPS.sendCommand("$PGCMD, 33, 0*6D"); //Turn off the antenna update to avoid nuisance signal/data.
 GPS.sendCommand(PMTK_SET_NMEA_UPDATE_RMCGGA); // Request RMC and GGA sentences only.
 GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ); //Set Update rate to 10Hz.
 delay(1000);

 SD.begin(chipSelect); // Initializing the SD card chipSelect pin.
 pinMode(10, OUTPUT); // Must declare 10 an output and reserve it to keep SD card.

 if(SD.exists("NMEA.txt")
 {
  SD.remove("NMEA.txt");
 }
 if(SD.exists("GPSData.txt")
 {
  SD.remove("GPSData.txt");
 }
}

void loop()
{
  readGPS();

     if(GPS.fix == 1)
     {
      mySensorData = SD.open("NMEA.txt", FILE_WRITE);
      mySensorData.println(NMEA1);
      mySensorData.println(NMEA2);
      mySensorData.close();
      mySensorData = SD.open("GPSData.txt",FILE_WRITE);
      mySensorData.print(GPS.latitude, 4); //Write measured latitude of 4 size to file.
      mySensorData.print(GPS.lat);
      mySensorData.print(",");
      mySensorData.print(GPS.longitude, 4);
      mySensorData.print(GPS.lon);
      mySensorData.print(",");
      mySensorData.print(GPS.altitude);
      mySensorData.close();
     }
}


void readGPS()
{
  clearGPS();
  while(!GPS.newNMEAreceived())
  {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
  NMEA1 = GPS.lastNMEA();

  while(!GPS.newNMEAreceived())
  {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA));
  NMEA2 = GPS.lastNMEA();

  Serial.println(NMEA1);
  Serial.println(NMEA2);
  Serial.println("");

}

void clearGPS()
{
  while(!GPSNMEAreceived())
  {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

  while(!GPSNMEAreceived())
  {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

  while(!GPSNMEAreceived())
  {
    c = GPS.read();
  }

  GPS.parse(GPS.lastNMEA());
}
