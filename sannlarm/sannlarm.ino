/*
AC dual dimmer Control for Sannlarm
adapted from sketch by Robert Twomey <rtwomey@u.washington.edu>
http://wiki.dxarts.washington.edu/groups/general/wiki/4dd69/
*/

#include <TimerOne.h>

volatile int i=0;
int AC_pin = 7;                 // Output to Opto Triac
int freqStep = 78;              // This is the delay-per-brightness step in microseconds. 65 uS for a 60Hz sinewave 

String serial_value;
unsigned char dimming = 85;     // Dimming level (5-100)
int dim = 128;                  // Dimming level (0-128)  0 = on, 128 = 0ff


// It is calculated based on the frequency of your voltage supply (50Hz or 60Hz)
// and the number of brightness steps you want.

// 1000000 us / 120 Hz = 8333 uS, length of one half-wave.
// 8333 uS / 128 steps = 65 uS / step

// 1000000 us / 100 Hz = 10000 uS, length of one half-wave.
// 10000 uS / 128 steps = 65 uS / step

void setup() {
  Serial.begin(9600);
  pinMode(AC_pin, OUTPUT);                        // Set the Triac pin as output
  attachInterrupt(1, zero_cross_detect, RISING);  // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                    // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);

  // Use the TimerOne Library to attach an interrupt to the function we use to check to see if it is the right time to fire the Triac
  // This function will now run every freqStep
}

void zero_cross_detect() {
  i=0;
  // since the control pin stays high, the Triac won't 'unlatch' when zero-crossing so I need to put the pins to LOW
  digitalWrite(AC_pin, LOW);
  // writing pins is like 10 times faster if we write the register directly instead of using 'digitalWrite'
}

// Turn on the Triac at the appropriate time
void dim_check() {
    if(i>=dim) {
      digitalWrite(AC_pin, HIGH); // turn on Triac
    }
    i++; // increment time step counter
}

void loop() {
  if(Serial.available()) {
    serial_value = Serial.readString();
    dimming = serial_value.toInt();
    Serial.println(dimming);
    dim = dimming;
  }
}
