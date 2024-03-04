/* Solenoid-Array.ino */
/* -----------------------------------------------------------------------------------
 >> System : Windows 11 w/ Arduino IDE
 >> References: https://docs.arduino.cc/ 
 >>             https://www.theengineeringprojects.com/2019/06/introduction-to-pn2222.html
 >>             https://www.youtube.com/watch?v=K04WPltjopc&t=5s
 >>             https://www.build-electronic-circuits.com/rectifier-diode/ (Diode)
 >>             https://users.ece.utexas.edu/~valvano/Datasheets/PN2222-D.pdf (Transistor)
 - -                             - -
 >> Brief Description   : Solenoid function - currently works with only two solenoids.
 >> Materials           : 2x PN2222 Transistor, 2x Diode Rectifier, 2x 1000 Ohm Resistor
 >>                       Many wires, 2x pretty bad solenoid (will change)
 >> Assumptions         : None
------------------------------------------------------------------------------------- */
int solenoidPin = 7;
int solenoidPin2 = 8;
void setup() {
    pinMode(solenoidPin, OUTPUT);
    pinMode(solenoidPin2, OUTPUT);
}

void loop()  {
    // HIGH: Pull Solenoid
    // LOW:  Release Solenoid
    digitalWrite(solenoidPin, HIGH); // Pull solenoid
    delay(250);
    digitalWrite(solenoidPin2, LOW); // Release solenoid
    delay(250);
    digitalWrite(solenoidPin, LOW); // Release solenoid
    delay(250);
    digitalWrite(solenoidPin2, HIGH); // PULL solenoid
    delay(250);
}