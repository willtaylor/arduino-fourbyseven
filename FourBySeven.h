/*
 *  FourBySeven.h - Drives a 4x7-segment display using 2 8-bit shift registers
 *  Author - Will Taylor
 *  2011.01.01
*/
#ifndef FourBySeven_h
#define FourBySeven_h

#include "WProgram.h"

class FourBySeven
{
  public:
    enum Marker { DP1, DP2, DP3, DP4, L1L2, L3 };
    FourBySeven(int serPin, int srclkPin, int rclkPin);
    void display(int value);
    void enableMarker(Marker marker);
    void disableMarker(Marker marker);
    void clearMarkers();
  private:
    void writeBits(unsigned short);
    int _serPin;
    int _srclkPin;
    int _rclkPin;
    int _value;
};

#endif
