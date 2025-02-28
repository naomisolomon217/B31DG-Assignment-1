#include <stdio.h>

#define greenLED 32  // green LED connected to GPIO32
#define redLED 33  // red LED connected to GPIO33
#define enable_button 27 //enable button connected to GPIO27
#define select_button 26 //select button connected to GPIO26

//define delays 
#define syncDelay 50
#define debounceDelay 50

//initial pulse-on time
const uint16_t a = 800; //us
//pulse-off time 
const uint16_t b = 1200; //us
//number of pulses 
const uint16_t c = 16; 
//time between end of final pulse off-time and start of next pulse cycle
const uint16_t d = 6000; //us = 6ms

bool toggleEnable = false; 
bool toggleSelect = true;

#define debugMode true 

#if debugMode 
#define timingFactor 1000
#else
#define timingFactor 1
#endif

void IRAM_ATTR selectISR()
{
  delayMicroseconds(debounceDelay);
  if (digitalRead(select_button))
  {
    toggleSelect = !toggleSelect;
  }
}

void IRAM_ATTR enableISR()
{
  delayMicroseconds(debounceDelay);
  if (digitalRead(enable_button))
  {
    toggleEnable = !toggleEnable;
  }
}

void setup() {

  Serial.begin(115200);

  // Define the LED pins
  pinMode(enable_button, INPUT_PULLUP);
  pinMode(select_button, INPUT_PULLUP);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
   
  attachInterrupt(digitalPinToInterrupt(enable_button), enableISR, RISING);
  attachInterrupt(digitalPinToInterrupt(select_button), selectISR, RISING);

}

void sig()
{
  for(int i = a; i <= 1550; i += 50)
  {
    digitalWrite(greenLED,toggleEnable);
    delayMicroseconds(i * timingFactor);
    digitalWrite(greenLED,  LOW);
    delayMicroseconds(b * timingFactor);
   }
   delayMicroseconds(d * timingFactor);
  }
    

void altSig()
{
  for(int i = 1550; i >= a; i -= 50)
  {
    digitalWrite(greenLED, toggleEnable);
    delayMicroseconds(i * timingFactor);
    digitalWrite(greenLED,  LOW);
    delayMicroseconds(b * timingFactor);
   }
   delayMicroseconds(d * timingFactor);
}


void loop() {
  while (toggleSelect)
  {
    digitalWrite(redLED, HIGH);
    delayMicroseconds(syncDelay * timingFactor);
    digitalWrite(redLED, LOW);
    sig();
  }


  while(!toggleSelect)
  {
    digitalWrite(redLED, HIGH);
    delayMicroseconds(syncDelay * timingFactor);
    digitalWrite(redLED, LOW);
    altSig();
  }

}