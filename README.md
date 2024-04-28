# Fire Protection System using Arduino Mega

### Software requirements

- Proteus (for circuit diagram and simulation)
- Arduino IDE (for Arduino code)

### Hardware requirements:
- Arduino Mega
- GSM & GPRS Module | SIM800L
- Gas Sensor | MQ-2
- Flame Sensor
- LCD | LM016L
- Speaker
- Relay
- 3 DC Motors
- LED
- 330ohm Resistor
  
### Objective: 

<p> The objectives of the microcontroller-based fire safety system project in an industrial setting can be divided into key areas to ensure comprehensive safety and functionality. The primary aim is to create a robust system that detects fire hazards early and initiates appropriate safety responses.

One of the main objectives is to provide fire detection and prevention. This involves using sensors to monitor for smoke, gas leaks, and other indicators of fire risk. By implementing this early warning system, the goal is to identify potential hazards before they escalate into larger incidents.

The second objective is to establish automated safety responses. Once a fire hazard is detected, the system should automatically shut off the main power source, close gas valves, and activate fire suppression systems. This automated response is designed to minimize damage and contain the hazard, thereby ensuring the safety of workers and reducing property damage.

Sending alerts and notifications to key personnel and local emergency services is another crucial objective. By providing immediate alerts to managers, safety officers, and nearby fire stations, the system ensures a rapid response from both internal and external resources. This objective enhances communication and coordination during emergencies.

Continuous monitoring and real-time feedback are also central to the project's objectives. The system must continuously monitor the industrial environment, providing real-time information to operators. This capability allows for quick action and evacuation if needed, contributing to a safer workplace.</p>


### Applications: 

<p>The applications of a microcontroller-based fire safety system extend across various industrial and commercial settings. This technology can be used to enhance safety, prevent disasters, and protect both human lives and valuable assets.</p>

### Block Diagram:

![Block Diagram](https://i.imgur.com/ZdFKOng.png)

<p><b>Fig: System Block Diagram</b></p>


### Working Procedures:
1.	The main component in the circuit is the Arduino Mega. It control everything.
2.	When the system is powered on it automatically starts to sense the environment for toxic gas or flames.
3.	The sensors will send these data to Arduino Mega. Arduino Mega will examine them according to the installed program.
4.	If the flame is detected the system will activate the emergency alarm, disconnect the building's primary power source, shut down the main gas valve to stop gas flow, activate an emergency high-pressure exhaust fan to remove leakage gas, and GSM & GPS module to send notification and position to the firefighter and authority.
5.	During gas leakage the gas valve will be shut off, the alarm & exhaust fan will be activated and notification will be sent.

### Schematic Circuit Diagram:

![Schematic Circuit Diagram](https://i.imgur.com/fj7g2FP.png)

<p>Fig: Schematic Circuit Diagram</p>


### Project Code:

```c

// Fire Protection System Using Arduino Mega

/* Includes Section*/
#include <Wire.h>
#include <SoftwareSerial.h>
#include "LiquidCrystal.h"

// Initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 53, en = 52, d4 = 51, d5 = 50, d6 = 49, d7 = 48;   
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                       

// Initialize an object from the class SoftwareSerial
SoftwareSerial sim800l(0, 1);       

/* Variables Section*/
#define Gas_Sensor_Output 22    // The output signal of the gas detector
#define Flame_Sensor_Output 23  // The output signal of the flame sensor
#define Emergency_LED 24
#define Emergency_Alarm 25
#define ELECTRIC_GAS_VALVE 26
#define FIRE_EXTINGUISHING_VALVE 27
#define Emergency_Exhaust_Fan 28
#define LOAD 29

unsigned long lcdTimer = 0;
unsigned long lcdInterval = 500;
unsigned long smsTimer = 0;
bool Flame_Sensor_state;
bool Gas_Sensor_state;

void setup() {
  /* Setting Pin Modes */
  pinMode(Flame_Sensor_Output, INPUT);
  pinMode(Gas_Sensor_Output, INPUT);
  pinMode(Emergency_Alarm, OUTPUT);
  pinMode(Emergency_LED, OUTPUT);
  pinMode(ELECTRIC_GAS_VALVE, OUTPUT);
  pinMode(FIRE_EXTINGUISHING_VALVE, OUTPUT);
  pinMode(Emergency_Exhaust_Fan, OUTPUT);
  pinMode(LOAD, OUTPUT);

  digitalWrite(LOAD, HIGH);                 // Connect the main building's electricity.
  digitalWrite(ELECTRIC_GAS_VALVE, HIGH);   // Activate the main gas valve

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("It's All good");

  // Begin the serial connection with baud rate 9600
  sim800l.begin(9600);
  Serial.begin(9600);

}

void loop() 
{
  Flame_Sensor_state = digitalRead(Flame_Sensor_Output);  // Check the output signal of the flame sensor.
  Gas_Sensor_state = digitalRead(Gas_Sensor_Output);      // Check the output signal of the gas sensor.

  // If the flame is detected
  if (Flame_Sensor_state == HIGH) 
  {
    /* Display on the LCD "Flame Detected!!"*/
    lcd.setCursor(0, 0);
    lcd.print("Fire Alert!!!!");
    lcd.setCursor(0, 1);
    lcd.print("Flame Detected!!");

    digitalWrite(Emergency_Alarm, HIGH);            // Activate the emergency alarm.
    digitalWrite(Emergency_LED, HIGH);              // Activate the emergency alarm.
    digitalWrite(LOAD, LOW);                        // Disconnect the building's primary power source.
    digitalWrite(ELECTRIC_GAS_VALVE, LOW);          // Shut down the main gas valve to stop gas flow.
    digitalWrite(FIRE_EXTINGUISHING_VALVE, HIGH);   // Activate the extinguishing valve to put down the fire.
    digitalWrite(Emergency_Exhaust_Fan, HIGH);      // Activate an emergency high-pressure exhaust fan to remove leakage gas.
    SendSMS();                                      // GSM & GPS module to send notification and position to the firefighter and authority.
  } 
  else if (Gas_Sensor_state == HIGH) 
  {
    /* Display on the LCD "Gas Detected!!"*/
    lcd.setCursor(0, 0);
    lcd.print("Fire Alert!!!!");
    lcd.setCursor(0, 1);
    lcd.print("Gas Detected!!");

    digitalWrite(Emergency_Alarm, HIGH);            // Activate the emergency alarm.
    digitalWrite(Emergency_LED, HIGH);              // Activate the emergency alarm.
    digitalWrite(ELECTRIC_GAS_VALVE, LOW);          // Shut down the main gas valve to stop gas flow.
    digitalWrite(Emergency_Exhaust_Fan, HIGH);      // Activate an emergency high-pressure exhaust fan to remove leakage gas.
    SendSMS();                                      // GSM & GPS module to send notification and position to the firefighter and authority.
  }
  else 
  {
    digitalWrite(Emergency_Alarm, LOW);             // Deactivate the emergency alarm.
    digitalWrite(Emergency_LED, LOW);               // Deactivate the emergency alarm.
    digitalWrite(LOAD, HIGH);                       // Connect the building's primary power source.
    digitalWrite(ELECTRIC_GAS_VALVE, HIGH);         // Open the main gas valve to stop gas flow.
    digitalWrite(FIRE_EXTINGUISHING_VALVE, LOW);    // Deactivate the extinguishing valve to put down the fire.
    digitalWrite(Emergency_Exhaust_Fan, LOW);       // Deactivate an emergency high-pressure exhaust fan to remove leakage gas.
    smsTimer = 0;                                   // Sets the smsTimer to 0;

    if (millis() - lcdTimer >= lcdInterval) 
    {
      lcd.clear();
      lcdTimer = millis();
      lcd.setCursor(0, 0);
      lcd.print("It's all good");
    }
  }
}

void SendSMS() 
{
  if(smsTimer == 0)
  {
    Serial.println("Sending Location...");
    sim800l.print("AT+CMGF=1\r");
    sim800l.print("AT+CMGS=\"180\"\r");
    sim800l.print("SIM8001 is working");
    sim800l.println();
    Serial.println("Location Sent.");
    smsTimer = millis();
  }
}
```

### Discussion:

<p>In the program first we import the 8051-family header file which contain all the necessary classes. Then we assign every connected pin to the variables. 
In keypad we assign variable as keyrow1 to keyrows4 for keypad rows and keycolumn1 to keycolumn3 for keypad columns. Then we connected the motor pins and lcd pins in the variable called motor pins and rs, rw and en.</p>

<p>After assign all the connected pins to the variables. We declare functions for every single task. The main function calls the other functions for execute the program.</p>

<p>Delay function is used for delay the program execution for few seconds. It helps us to execute the program smoothly. We delay the execution for only 1000ms = 1secs. </p>

<P>The lcdcmd function is help us to control the lcd. It will control the current flow for the lcd screen and also help to perform Read/Write Operation. 
The lcddat function control the lcd data pins. In the LCD there is total 8 pins D0 to D7 Pins used to send Command or data to the LCD.</P>

<p>The lcddisplay function is used for display user input characters and also assign the values for every keypad character values. First, we select a single keypad row and assign the values for the key. Also encoded with the asterisk (*) character, for pin protection.</p>

<p>The check function is used for compare the user input with the assign password number. If the user input match the assign password, it will show the “pin correct” message and give the signal to run the lock motor. After lock motor will run, again it will show the “door open” message. On the other hand, if the user input password is wrong, it will show the “wrong pin” message. Then it will delay few second and give another chance to input the password again.</p>

<p>The main function is the mother function of the program. Main function will call other function in other to perform the task. At first it will call the lcdcmd function to control the lcd, then it will call the lcddisplay function for display the messages in the lcd screen. It will delay for few seconds. After that it will run loop until the correct pin enter. To check the password is correct it will call check function, then execute the program.</p>

<p>To simulate the program circuit, we use the proteus simulation. All the code is written in embedded C code, which is not directly executed by the 8051 microcontrollers. To run the program first we need to build the program and convert the program in hex file. The hex file is supported by the 8051 microcontrollers.  Every 8051 controller has a clock frequency for perform the task. To run the program, we use 11.0592 MHZ clock frequency. It will run the simulation smoothly without any kind of errors.</p>
