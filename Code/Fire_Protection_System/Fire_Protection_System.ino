/* Includes Section*/
#include <Wire.h>
#include <SoftwareSerial.h>
#include "LiquidCrystal.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 53, en = 52, d4 = 51, d5 = 50, d6 = 49, d7 = 48;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

// initialize an object from the class SoftwareSerial
SoftwareSerial sim800l(0,1);

/* Variables Section*/
#define Gas_Sensor_Output 22            // The output signal of the gas detector
#define Flame_Sensor_Output 23          // The output signal of the flame sensor
#define Emergency_LED 24                
#define Emergency_Alarm 25              
#define ELECTRIC_GAS_VALVE 26           
#define FIRE_EXTINGUISHING_VALVE 27     
#define Emergency_Exhaust_Fan 28
#define LOAD 29       

unsigned long lcdTimer = 0;
unsigned long lcdInterval = 500;
bool Flame_Sensor_state;
bool Gas_Sensor_state;

void setup() 
{
  /* Setting Pin Modes */
  pinMode(Flame_Sensor_Output, INPUT);
  pinMode(Gas_Sensor_Output, INPUT);
  pinMode(Emergency_Alarm, OUTPUT);
  pinMode(Emergency_LED, OUTPUT);
  pinMode(ELECTRIC_GAS_VALVE, OUTPUT);
  pinMode(FIRE_EXTINGUISHING_VALVE, OUTPUT);
  pinMode(Emergency_Exhaust_Fan, OUTPUT);
  pinMode(LOAD, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("It's All good");

  sim800l.begin(9600);
  Serial.begin(9600);
  delay(1000);
}

void loop() 
{
  Flame_Sensor_state = digitalRead(Flame_Sensor_Output);
  Gas_Sensor_state = digitalRead(Gas_Sensor_Output);

  if(Flame_Sensor_state == HIGH || Gas_Sensor_state == HIGH)
  {
    Serial.println("Fire Alert!!!!");

    if(Flame_Sensor_state == HIGH)
    {
      lcd.setCursor(0, 0); 
      lcd.print("Fire Alert!!!!");
      lcd.setCursor(0,1);
      lcd.print("Flame Detected!!");
    }
    else if(Gas_Sensor_state == HIGH)
    {
      lcd.setCursor(0, 0); 
      lcd.print("Fire Alert!!!!");
      lcd.setCursor(0,1);
      lcd.print("Gas Detected!!");
    }
    else
    {
      lcd.setCursor(0, 0); 
      lcd.print("Fire Alert!!!!");
      lcd.setCursor(0,1);
      lcd.print("Gas&Flame Found!");
    }
    
    digitalWrite(Emergency_Alarm, HIGH);
    digitalWrite(Emergency_LED, HIGH);
    digitalWrite(ELECTRIC_GAS_VALVE, HIGH);
    digitalWrite(FIRE_EXTINGUISHING_VALVE, HIGH);
    digitalWrite(Emergency_Exhaust_Fan, HIGH);
    digitalWrite(LOAD, LOW);

    SendSMS();
  }
  else
  {
    if (millis() - lcdTimer >= lcdInterval)
    {
      lcd.clear();
      lcdTimer = millis();
      lcd.setCursor(0, 0);
      lcd.print("It's all good");
    }

    digitalWrite(Emergency_Alarm, LOW);
    digitalWrite(Emergency_LED, LOW);
    digitalWrite(ELECTRIC_GAS_VALVE, LOW);
    digitalWrite(FIRE_EXTINGUISHING_VALVE, LOW);
    digitalWrite(Emergency_Exhaust_Fan, LOW);
    digitalWrite(LOAD, HIGH);
  }

}

void SendSMS()
{
  Serial.println("Sending Location...");
  sim800l.print("AT+CMGF=1\r");
  delay(100);
  sim800l.print("AT+CMGS=\"+201022546004\"\r");
  delay(100);
  sim800l.print("SIM8001 is working");
  delay(100);
  sim800l.println();
  Serial.println("Location Sent.");
  delay(100);
}