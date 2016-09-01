 /*
Analog Steering Wheel Controls Interface

This uses an Analog input to measure the button presses
then sends the appropiate commands to an Android tablet
via keyboard HID shortcuts.

Requires https://github.com/NicoHood/HID

The values below are what I measured. If yours differs
use the analog to serial example sketch to retrieve
new numbers.

Vol Up 47-49
Vol Down 55-57
Seek Up 75-77
Seed Down 63-65
Source Up 167-169
Source Down 282-283
Talk 92-94
Prog 118-120 
normal 1018
with input_pullup

On a 05 chevy silverado the two wires are connected into the BCM
on Connector 3 (Brown) Pin A7 (Lite Green), 
and Connector 2 (Grey) Pin A2 (Dark Blue).
I cut the wires just short of the connectors and spliced them
to wires going to GND and A0 on the Leonardo.
*/

#include "HID-Project.h"

const int pinLed = LED_BUILTIN;
const int inputPin = A0;

int volumeUp = 1;
int volumeDown = 2;
int seekUp = 3;
int seekDown = 4;
int sourceUp = 5;
int sourceDown = 6;
int talk = 7;
int prog = 8;

int sensorValue = 0;
int lastSensorValue = 0;
int buttonPress = 0;
int keyHeld = 0;
unsigned long keyHeldTimer = 0;

void readSensor() { //This reads the input and makes sure that it's stable before continuing
    lastSensorValue = sensorValue;
    sensorValue = analogRead(inputPin);
  while ( ! ( sensorValue < lastSensorValue + 3 && sensorValue > lastSensorValue - 3)) {
    lastSensorValue = sensorValue;
    sensorValue = analogRead(inputPin);
      delay(10);
  }

  if (sensorValue >= 45 && sensorValue < 51) {buttonPress = volumeUp;}
  else if (sensorValue >= 51 && sensorValue < 60) {buttonPress = volumeDown;}
  else if (sensorValue >= 60 && sensorValue < 70) {buttonPress = seekDown;}
  else if (sensorValue >= 70 && sensorValue < 85) {buttonPress = seekUp;}
  else if (sensorValue >= 85 && sensorValue < 105) {buttonPress = talk;}
  else if (sensorValue >= 105 && sensorValue < 145) {buttonPress = prog;}
  else if (sensorValue >= 145 && sensorValue < 200) {buttonPress = sourceUp;}
  else if (sensorValue >= 200 && sensorValue < 300) {buttonPress = sourceDown;}
  else {buttonPress = 0;}
}

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(inputPin, INPUT_PULLUP);
  Consumer.begin(); //Consumer HID keyboard init
  System.begin(); //System HID keyboard init
  BootKeyboard.begin(); //Boot standard HID keyboard init
  
  //  The following code is to unlock your tablet if you choose to use a pin code.
  //  delay(4000); //Make sure to give your device enough time to wake up
  //  BootKeyboard.write(KEY_SPACE);
  //  delay(1000);
  //  BootKeyboard.println("123456"); //Unlock code for your device
  //  delay(100);
}

void loop() {
  unsigned long currentMillis = millis();
  readSensor(); //Read the current button state

  // For application tabbing, we check to see if the Alt key
  // is still being held
  if (keyHeld && keyHeldTimer > currentMillis - 5000) {
    if (buttonPress == sourceUp) {
     digitalWrite(pinLed, HIGH);
     keyHeldTimer = millis();
     BootKeyboard.press(KEY_LEFT_ALT);
     delay(100);
     BootKeyboard.press(KEY_TAB);
     delay(100);
     BootKeyboard.release(KEY_TAB);
     delay(300);
     digitalWrite(pinLed, LOW);
     }
   }

   //If the key hold timer has expired, release it
   else if (keyHeld) {
    digitalWrite(pinLed, HIGH);
    BootKeyboard.releaseAll();
    keyHeld = 0;
    delay(300);
     digitalWrite(pinLed, LOW);
   }

   //Now on to just detecting key presses
  else if (buttonPress == talk) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_VOLUME_MUTE);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  else if (buttonPress == volumeUp) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_VOLUME_UP);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  else if (buttonPress == volumeDown) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_VOLUME_DOWN);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  else if (buttonPress == seekUp) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_NEXT);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  else if (buttonPress == seekDown) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_PREVIOUS);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  else if (buttonPress == prog) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_PLAY_PAUSE);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  else if (buttonPress == sourceDown) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(CONSUMER_BROWSER_HOME);
    delay(300);
    digitalWrite(pinLed, LOW);
  }

  //Here is the Application switching. It's part of
  //a two part process as it requires the alt key to
  //be held for a period of time.
  else if (buttonPress == sourceUp) {
    digitalWrite(pinLed, HIGH);
    keyHeld = 1;
    keyHeldTimer = millis();
    BootKeyboard.press(KEY_LEFT_ALT);
    delay(100);
    BootKeyboard.press(KEY_TAB);
    delay(100);
    BootKeyboard.release(KEY_TAB);
    delay(300);
    digitalWrite(pinLed, LOW);
  }

}
