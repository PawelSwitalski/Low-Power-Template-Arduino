#include <Arduino.h>

// Default LED
const int ledPin = 13;

// Define the time interval for blinking in milliseconds
const int blinkInterval = 1000; // 1000 milliseconds = 1 second

void digitalInterrupt();
ISR(WDT_vect);

void setup() {
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);

  // Save Power by writing all Digital IO LOW
  for (int i = 0; i < 20; i++)
  {
    if (i != ledPin)
    {
      pinMode(i, OUTPUT);
    }
      
  }
  
  attachInterrupt(0, digitalInterrupt, FALLING); // interrupt for waking up
}

void loop() {

  // Example blinking LED
  digitalWrite(ledPin, HIGH);
  delay(blinkInterval);
  digitalWrite(ledPin, LOW);
  delay(blinkInterval);

  // SETUP WATCHDOG TIMER
  WDTCSR = (24); // change enable and WDE - also resets
  WDTCSR = (33); // prescalers only - get ride of the WDE and WDCE bit
  WDTCSR |= (1 << 6); // enable interrupt mode


  // Disable ADC
  ADCSRA &= ~(1 << 7);
  // ENABLE SLEEP
  SMCR |= (1 << 2); // power down mode
  SMCR |= 1; // enable sleep

  // BOD disable
  MCUCR |= (3 << 5); // set both BODS and BODSE at the same time
  MCUCR |= (MCUCR & ~(1 << 5)) | (1 << 6); // set the BODS bit and clear the BODSE bit at the same time

  __asm__ __volatile__("sleep");
}

void digitalInterrupt(){

}

ISR(WDT_vect){} // watchdog interrupt