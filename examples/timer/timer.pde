#include <FourBySeven.h>

// Shift Register input
const int SER_PIN = 10;
const int RCLK_PIN = 9;
const int SRCLK_PIN = 8;

FourBySeven fourBySeven(SER_PIN, SRCLK_PIN, RCLK_PIN);
long timer;
long timer2;
long startup;
boolean flipFlop = false;
int hours = 0;
int minutes = 0;
int seconds = 0;

void setup() {
  startup = millis();
  timer = 0;
  timer2 = 0;
  fourBySeven.enableMarker(FourBySeven::L1L2);
}

void loop() {
  if ( seconds == 60 ) {
    seconds = 0;
    ++minutes;
  }
  
  if ( minutes == 60 ) {
    minutes = 0;
    ++hours;
  }
  
  if ( hours > 99 ) {
    hours = 0;
    minutes = 0;
    seconds = 0;
  }
 
  fourBySeven.display(hours * 100 + minutes);

  long now = millis();
  long temp = (now - startup) / 1000;
  if ( temp > timer ) {
    ++seconds;
    timer = temp;
  }
  temp = (now - startup) / 500;
  if ( temp > timer2 ) {
    if ( flipFlop ) {
      fourBySeven.enableMarker(FourBySeven::L1L2);
    } else {
      fourBySeven.disableMarker(FourBySeven::L1L2);
    }
    flipFlop = !flipFlop;
    timer2 = temp;
  }
}
