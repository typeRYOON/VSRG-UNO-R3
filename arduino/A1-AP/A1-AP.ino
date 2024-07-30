/* A1-AP.ino */
/* ---------------------------------------------------------------------------------------
 >> Assignment details and provided code are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: A1-AP.ino :: VSRG-UNO-R3: Arduino 1, Photoresistor Master I2C
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: John Ezra See
 >> UIN/nID :: 668942698 (jsee4)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file implements Arduino 1. This program controls a photoresistor array and sends
 >> the osu mania key binary mapping to the GUI. This program is meant to be run with the
 >> VSRG-UNO-R3 GUI. Link below
 - -              >> https://github.com/typeRYOON/VSRG-UNO-R3 <<                       - -
 >> Please read the instructions on the github page to understand how to run this
 >> Arduino. Hardware required is listed in the repository README/documentation files.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include <Wire.h>

// Global variables :: - -                                                             - -
int32_t light, buttonState = 1, buttonPin = 8, buttonStateOutput = 9;
const uint8_t  pinValues[] = { A0, A1, A2, A3, 7, 6, 5, 4 };
const uint16_t luxValues[] = { 550, 550, 550, 550 }; // Calibrate as needed
uint8_t  columnHits, curButton = LOW, prevButton = LOW;
uint32_t prevTime = 0, curTime;

// Set up all the pins :: - -                                                          - -
void setup()
{
  Serial.begin( 9600 );
  for ( uint8_t i = 4; i < 8; ++i ) {
    pinMode( pinValues[i], OUTPUT );
  }
  pinMode( buttonPin, INPUT );  
  pinMode( buttonStateOutput, OUTPUT );
  Wire.begin();
} 

// Main Arduino program loop :: - -                                                    - -
void loop()
{
  curTime = millis();
  curButton = digitalRead( buttonPin );

  if ( curTime - prevTime < 20 ) { return; }

  // Debounce:
  if ( prevButton == LOW && curButton == HIGH ) {
    buttonState = !buttonState;

    // Send to Arduino 2 only when button is pressed:
    // buttonState = 0 -> OFF
    // buttonState = 1 -> ON
    Wire.beginTransmission( 5 );
    Wire.write( buttonState );
    Wire.endTransmission();
  }

  prevTime   = curTime;
  prevButton = curButton;

  columnHits = 0;
  // Read all photoresistors and write to LEDs :: - -                                  - -
  for ( uint8_t i = 0; i < 4; ++i )
  {
    light = analogRead( pinValues[i] );
    if ( buttonState == 0 ) { light = 0; } // If button is OFF, photoresistor is OFF.
    if ( light >= luxValues[i] )
    {
      columnHits |= 1 << i;
      digitalWrite( pinValues[i + 4], HIGH );
    }
    else {
      digitalWrite( pinValues[i + 4], LOW  );
    }
  }
  Serial.print( columnHits ); // Send column mapping to GUI.
} // :: - -                                                                            - -
