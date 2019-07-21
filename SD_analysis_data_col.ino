#include <SD.h>
#include <SPI.h>
File myFile;
#define RedLED 6
#define GreenLED 7
#define YellowLED 8
#define button 9
#define AnalogPin A0
int pinSS = 10;
unsigned long int temp_var = 0;
unsigned long ultimate_var = 0;
int DigitalValue = 0;
byte samples = 0, value = 0;
Buttoncontrol = 0;
control = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(YeelowLED, OUTPUT);
  pinMode(pinSS, OUTPUT);
  if (SD.begin())
  {
   Serial.println("SD card Initialized."); 
  }
  else
  {
    Serial.println("SD card not yet Initialized");
    digitalWrite(RedLED, HIGH);
  }
 }

void loop()
{
  bool pin = digitalRead(button);
  if(pin == 1 && control == 0 && Buttoncontro == 0)
  {
    myFile = SD.open("Saple.txt", FILE_WRITE); //Create, open the file Samples.txt
    delay(500);
    Buttoncontrol = 1;
    value++;
    myFile.print("sample : ")
    myFile.println(value);
    samples = 0;
    temp_var = millis();
  }
    if(Buttoncontrol == 1) && (temp_var - ultimate_var >= 1000) && (samples < 10 && control ==1)
    {
      DigtialValue = analogRead(AnalogPin);
      myFile.println(DigitalValue);
      control = 0;
      ultimate_var = temp_var;
      samples++;
     digitalWrite(YeelowLED, HIGH);
    }
     
     if(samples >= 10)
     {
      digitalWrite(YeelowLED, LOW);
      myFile.close();
      BUttoncontrol = 0;
      digitalWrite(GreenLED, HIGH);
     }
     control = control ^ 1;
}

 
