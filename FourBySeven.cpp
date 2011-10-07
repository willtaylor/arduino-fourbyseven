/*
 * FourBySeven.cpp - Drives a 4x7-segment display using 2 8-bit shift registers
 *  Author - Will Taylor
 *  2011.01.01
 */
#include "WProgram.h"
#include "FourBySeven.h"

// bit masks for mapping 4x7 display input to shift register output
#define D1_BIT     0b1000000000000000  // 1
#define D2_BIT     0b0100000000000000  // 2
#define L1_L2_BIT  0b0010000000000000  // 3
#define D3_BIT     0b0001000000000000  // 4
#define D4_BIT     0b0000100000000000  // 5
#define G_BIT      0b0000010000000000  // 6
#define DP_BIT     0b0000001000000000  // 7
#define E_BIT      0b0000000100000000  // 8
#define B_BIT      0b0000000010000000  // 9
#define A_BIT      0b0000000001000000  // 10
#define C_BIT      0b0000000000100000  // 11
#define L1_L2_BIT2 0b0000000000010000  // 12
#define F_BIT      0b0000000000001000  // 13
#define L3_BIT     0b0000000000000100  // 14
#define D_BIT      0b0000000000000001  // 15
#define L3_BIT2    0b0000000000000010  // 16

#define SEGMENTS A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT | G_BIT | DP_BIT

// defined by what segments are turned HIGH (off) for each number
#define N_0 G_BIT | DP_BIT
#define N_1 A_BIT | D_BIT | E_BIT | F_BIT | G_BIT | DP_BIT
#define N_2 C_BIT | F_BIT | DP_BIT
#define N_3 E_BIT | F_BIT | DP_BIT
#define N_4 A_BIT | D_BIT | E_BIT | DP_BIT
#define N_5 B_BIT | E_BIT | DP_BIT
#define N_6 A_BIT | B_BIT | DP_BIT
#define N_7 D_BIT | E_BIT | F_BIT | G_BIT | DP_BIT
#define N_8 DP_BIT
#define N_9 D_BIT | E_BIT | DP_BIT

const unsigned short positions[] = { D1_BIT, D2_BIT, D3_BIT, D4_BIT };
const unsigned short segments[] = { A_BIT, B_BIT, C_BIT, D_BIT, E_BIT, F_BIT, G_BIT };
const unsigned short numbers[] = { N_0, N_1, N_2, N_3, N_4, N_5, N_6, N_7, N_8, N_9 };
const unsigned short segmentMask = SEGMENTS;

int _serPin;
int _srclkPin;
int _rclkPin;

unsigned short markerBits = segmentMask & ~(DP_BIT | L1_L2_BIT2 | L3_BIT);

FourBySeven::FourBySeven(int serPin, int srclkPin, int rclkPin)
{
  pinMode(serPin, OUTPUT);
  pinMode(srclkPin, OUTPUT);
  pinMode(rclkPin, OUTPUT);

  _serPin = serPin;
  _srclkPin = srclkPin;
  _rclkPin = rclkPin;
}

void FourBySeven::display(int value)
{
  int digits[4];
  int temp = value;
  for ( int i = 3; i >= 0 ; --i ) {
    digits[i] = temp % 10;
    temp = temp / 10;
  }

  for ( int i = 0; i < 4; ++i ) {
    int digit = digits[i];
    for ( int j = 0; j < 7; ++j ) {
      unsigned short segmentBits = (segmentMask & ~segments[j]) | numbers[digit];
      unsigned short outputBits = positions[i] | segmentBits;
      writeBits(outputBits);
    }
  }
  writeBits(markerBits);
}

void FourBySeven::enableMarker(Marker marker) {
  switch ( marker ) {
    case FourBySeven::DP1:
      markerBits = markerBits | D1_BIT;
      break;
    case FourBySeven::DP2:
      markerBits = markerBits | D2_BIT;
      break;
    case FourBySeven::DP3:
      markerBits = markerBits | D3_BIT;
      break;
    case FourBySeven::DP4:
      markerBits = markerBits | D4_BIT;
      break;
    case FourBySeven::L1L2:
      markerBits = markerBits | L1_L2_BIT;
      break;
    case FourBySeven::L3:
      markerBits = markerBits | L3_BIT;
      break;
  }
}

void FourBySeven::disableMarker(Marker marker) {
  switch ( marker ) {
    case FourBySeven::DP1:
      markerBits = markerBits & ~D1_BIT;
      break;
    case FourBySeven::DP2:
      markerBits = markerBits & ~D2_BIT;
      break;
    case FourBySeven::DP3:
      markerBits = markerBits & ~D3_BIT;
      break;
    case FourBySeven::DP4:
      markerBits = markerBits & ~D4_BIT;
      break;
    case FourBySeven::L1L2:
      markerBits = markerBits & ~L1_L2_BIT;
      break;
    case FourBySeven::L3:
      markerBits = markerBits & ~L3_BIT;
      break;
  }
}

void FourBySeven::clearMarkers() {
  markerBits = segmentMask & ~(DP_BIT | L1_L2_BIT2 | L3_BIT);
}

void FourBySeven::writeBits(unsigned short bits) {
  digitalWrite(_rclkPin, LOW);

  for ( int i = 0; i < 16; ++i ) {
    digitalWrite(_srclkPin, LOW);
    digitalWrite(_serPin, 1 & bits);
    digitalWrite(_srclkPin, HIGH);
    bits = bits >> 1;
  }

  digitalWrite(_rclkPin, HIGH);
}
