#include<SPI.h>
#include<DHT.h>
#include<SoftwareSerial.h>
#include<RH_ASK.h>

SoftwareSerial mySerial(4, 3);

#define DHTPIN 2
#define DHTTYPE DHT11

int smokeA0 = A5;
int sensorThreshold = 400;

DHT dht(DHTPIN, DHTTYPE);
//initializing DHT sensor for 16Mhz clock for Arduino.

float humidit;
float temperature;

//Output Strings=>
String str_humidit;
String str_temperature;
String str_smoke;
String str_out;

//Amplitude Shift Keying Object.

RH_ASK rf_driver;


void setup(){
pinMode(smokeA0, INPUT);
dht.begin();
//Initializing ASK object.
rf_driver.init();
Serial.begin(9600);
mySerial.begin(115200);  
}

void loop(){
  delay(2000);
  humidit = dht.readHumidity();
  temperature = dht.readTemperature();
  Serial.println("Reciever Humidity = ");
  Serial.print(humidit);
  Serial.println("\n");
  Serial.print("Receiver Temperature = ");
  Serial.print(temperature);

  int analogSensor = analogRead(smokeA0);
  String smoke;

  if (analogSensor > sensorThreshold){
    Serial.print("Smoke at Receiver");
    Serial.print("\n");
    smoke = "Smoke";
  }
  else{
    Serial.print("Clean at Reciever");
    Serial.print("\n");
    smoke = "Clean";
  }

  uint8_t buf[20];
  uint8_t buflen = sizeof(buf);

  //Check if the recieved packet is of correct size.
  if (rf_driver.recv(buf, &buflen))
  {
    str_out = String((char*)buf);
    for (int i = 0; i < str_out.length(); i++){
      if (str_out.substring(i, i+1) == ",")
      {
        str_humidit = str_out.substring(0, i);
        str_temperature = str_out.substring(i + 1, i + 6);
        str_smoke = str_out.substring(i + 7, i + 12);
        break;
        
      }
    }
  }

  Serial.print("Humidity = ");
  Serial.print(str_humidit);
  Serial.print(" Temperature = ");
  Serial.println(str_temperature);
  Serial.print("Air Quality = ");
  Serial.println(str_smoke);

  if (humidit >= 0 && temperature >= 25){
    Serial.print("Fire Detected at Reciever ");
    mySerial.println("AT + CMGF = 1");  //Sets the GSM Module into Text Mode.
    delay(1000);
    mySerial.println("AT+CMGS =\"+919711771792\"\r";
    delay(1000);
    mySerial.println("  FIRE ALERT!  \n");
    mySerial.println("FIRE AT RECIEVER NODE \n");
    mySerial.println("Temperature = " + String(temperature));
    Serial.println("\n");
    mySerial.println("AIR Quality = " + smoke);
    delay(100);
    mySerial.println((char)26); //ASCII Code for Ctrl + Z
    delay(1000);
    }
    if (str_humidit.toInt() >= 60 && str_temperature.toInt() >= 25){
       Serial.print("Fire Detected at Transmitter End !");
       mySerial.println("AT + CMGF = 1"); //Sets the GSM Module into Text Mode.
       delay(1000);
       mySerial.println("AT + CMGS = \"+919711771792\"r");
       delay(1000);
       mySerial.println("  FIRE ALERT!  \n");
       mySerial.print("\n");
       mySerial.println("Fire at Transmitter end! \n");
       mySerial.println("Temperature : + str_temperature);
       Serial.println("\n");
       mySerial.println("AIR Quality = " + str_smoke);
       delay(100);
       mySerial.println((char)26); //ASCII Code for CTRL + Z
       delay(1000);
    }
}

//Transmitter Part =>

#include <SPI.h>
#include <DHT.h>
#include <RH_ASK.h>

#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

int smokeA0 = A5;
int sensorthrehold = 400;

float humidit; //in percent.
float temperature; //in degree Celcius.
float smoke; //normal float value

String out, str_temperature, str_humidit, str_smoke;

//Creating Amplitude Shift Keying Object.
RH_ASK rf_driver;

void setup()
{
  dht.begin();
  Serial.begin(9600);
  pinMode(smokeA0, INPUT);
  rf_driver.init();
}


void loop()
{
delay(2000);

humidit = dht.readHumidity();
temperature = dht.readTemperature();

str_humidit = String(humidit);
Serial.print(humidit);

str_temp = String(temperature);
Serial.print(temperature);

int analogSensor = analogRead(smokeA0);

if(analogSensor >= sensorThreshold){
  str_smoke = "1";
}
else{
  str_smoke = "0";
    }
    if (str_smoke == "1"){
      str_out = str_humid + "," + str_temp + "," + "Smoke";
      }
     if (str_smoke == "0"){
     str_out = str_humidit + "," + str_tmperature + "," + "Clean";
     }
     const char *msg = str_out.c_str();

          rf_driver.send((uint8_t *)msg, strlen(msg));
          tf_driver.waitPacketSent();

}
