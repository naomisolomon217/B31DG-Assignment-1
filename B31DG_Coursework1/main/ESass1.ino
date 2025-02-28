#include <stdio.h>

//GPIO definitions
#define greenLED 32  // green LED connected to GPIO32
#define redLED 33  // red LED connected to GPIO33
#define enable_button 27 //enable button connected to GPIO27
#define select_button 26 //select button connected to GPIO26

//define delay times
#define syncDelay 50  //synchronization delay between pulses (us)
#define debounceDelay 50 //debounce delay for button presses (us)

//pulse timing parameters 
const uint16_t a = 800; //initial pulse-on time (us)
const uint16_t b = 1200; //pulse-off time (us)
//number of pulses c = 16; 

//time between end of final pulse off-time and start of next pulse cycle
const uint16_t d = 6000; //us = 6ms

//state flags for button presses 
bool toggleEnable = false; //flag to control enable state 
bool toggleSelect = true; //flag to control select state 

//debugging flag
#define debugMode true 

//timing factor for delay scaling 
#if debugMode 
#define timingFactor 1000 //scale by factor of 1000 for debugging 
#else
#define timingFactor 1 //regular operation (no scaling)
#endif

//Interrupt Service Routine (ISR) for select button 
void IRAM_ATTR selectISR()
{
  delayMicroseconds(debounceDelay); //debounce button press
  if (digitalRead(select_button)) //check if select button is pressed 
  {
    toggleSelect = !toggleSelect; //toggle select state 
  }
}

//Interrupt Service Routine (ISR) for enable button 
void IRAM_ATTR enableISR()
{
  delayMicroseconds(debounceDelay); //debounce button press
  if (digitalRead(enable_button)) //check if enable button is pressed 
  {
    toggleEnable = !toggleEnable; //toggle enable state 
  }
}

void setup() {
  //start serial communication for debugging (at correct baud rate)
  Serial.begin(115200);

  // Define the GPIO pins
  pinMode(enable_button, INPUT_PULLUP); //set enable button pin as input woith pull-up resistor
  pinMode(select_button, INPUT_PULLUP); //set select button pin as input woith pull-up resistor
  pinMode(redLED, OUTPUT); //set red LED pin as output 
  pinMode(greenLED, OUTPUT); //set green LED pin as output 
   
  //attach interrupts for button presses 
  attachInterrupt(digitalPinToInterrupt(enable_button), enableISR, RISING); //interrupt on rising edge of enable button 
  attachInterrupt(digitalPinToInterrupt(select_button), selectISR, RISING); //interrupt on rising edge of select button

}

//signal function (green LED flashes 16 times, getting longer each flash)
void sig()
{
  for(int i = a; i <= 1550; i += 50) //loop 16 times, in increments of 50
  {
    digitalWrite(greenLED, toggleEnable); //turn green LED on
    delayMicroseconds(i * timingFactor);  //pulse-on delay 
    digitalWrite(greenLED,  LOW);         //turn LED off
    delayMicroseconds(b * timingFactor);  //pulse-off delay off
   }
   delayMicroseconds(d * timingFactor);   //delay between cycles 
  }
    
//alternative signal function (green LED flashes 16 times, getting shorter each flash)
void altSig()
{
  for(int i = 1550; i >= a; i -= 50) //loop 16 times, in decrements of 50
  {
    digitalWrite(greenLED, toggleEnable); //turn green LED on 
    delayMicroseconds(i * timingFactor);  //pulse-on delay 
    digitalWrite(greenLED,  LOW);         //turn LED off 
    delayMicroseconds(b * timingFactor);  //pulse-off delay 
   }
   delayMicroseconds(d * timingFactor);   //delay between cycles 
}


void loop() {

  //when toggleSelect is true, flash red LED and trigger signal function 
  while (toggleSelect)
  {
    digitalWrite(redLED, HIGH); //LED on 
    delayMicroseconds(syncDelay * timingFactor); //synchronisaton delay 
    digitalWrite(redLED, LOW); //LED off
    sig(); //calls sig() function 
  }


  while(!toggleSelect)
  {
      //when toggleSelect is flase, flash red LED and trigger alternative signal function 

    digitalWrite(redLED, HIGH); //LED on 
    delayMicroseconds(syncDelay * timingFactor); //sync delay
    digitalWrite(redLED, LOW); //LED off
    altSig(); //calls altSig() function 
  }

}