/* rmagd2-Lab-04.ino */
/* ---------------------------------------------------------------------------------------
 >> Assignment details and provided code are created and owned by Patrick Troy.
 >> University of Illinois Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File   : rmagd2-Lab-04.ino :: Lab 4 - Photoresistor (LDR – Light Dependent Resistor)
 >> Author : Ryan Magdaleno
 >> UIN/nID: 668523658 (rmagd2)
 >> System : Windows 10 w/ Mingw-w64
 >> TA     : 
 - -                             - -
 >> No References used ::
 - -                             - -
 >> File overview ::
 >> 
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include <LiquidCrystal.h>

// Global variables ::
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char buffer[16];
int value;

void setup()
{
  // Set up devices ::
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("                ");
}


void loop()
{
  lcd.setCursor(0, 0);
  if (Serial.available()) {
    value = Serial.read() - 1;

    // Discard the next 7 bytes
    for (int i = 0; i < 7; ++i) {
      if (Serial.available() > 0) { Serial.read(); }
      else { break; }
    }
    sprintf(buffer, "        %d        ", value);
    lcd.println(buffer);
  }
}