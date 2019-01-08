#include <SoftwareSerial.h>
#include "String.h"

char send_val = ' ';
//const int RxD = 8;
//const int TxD =7;
const int pinButton = 2;
const int greenLED = 12;
const int redLED = 13;
const int offBUTTON = 3;

SoftwareSerial  BTSerial(7, 8); //RXD TXD

int val = 0; 
int old_val = 0;
int state = 0; 

int val2 = 0;
int old_val2 = 0;
int activeState = 0;

boolean active;

void setup() {
BTSerial.begin(9600); 
Serial.begin(9600);
pinMode (pinButton, INPUT);
pinMode(greenLED, OUTPUT); 
pinMode(redLED, OUTPUT); 
pinMode(offBUTTON, INPUT);
//pinMode(RxD,INPUT);
//pinMode(TxD,OUTPUT); 
}

void loop() {
int state;
int activeState;


val2 = digitalRead(offBUTTON);
activeState = digitalRead(offBUTTON);

val = digitalRead(pinButton);
state = digitalRead(pinButton);


if (activeState == HIGH) {
  if (active == true) {
    active = false;
  }
  else{
  if(active == false){
    active = true;
  }
  }
  delay(300);
}


if (state == HIGH && active == true)
{
  send_val = '1';
  BTSerial.println(send_val); //Data to be transmitted over Bluetooth
  Serial.print(send_val);
}
else
{
  send_val = '0';
  BTSerial.println(send_val); //Data to be transmitted over Bluetooth
  Serial.print(send_val);
  // Do Nothing
}

if ((val == HIGH) && (old_val == LOW)){
state = 1 - state;
delay(100);
}
old_val = val;

if (state == 1) {
if (active == true) {
digitalWrite(greenLED, HIGH); // turn LED ON
}
else {
  digitalWrite(redLED, HIGH);
}
} else {
digitalWrite(greenLED, LOW);
digitalWrite(redLED, LOW);
}


delay(10);
}
