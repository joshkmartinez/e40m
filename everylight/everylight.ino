/* Starter code to blink every LED using the simplest possible iteration
 * through anode and cathode pins.
 *
 * ENGR 40M
 * July 2018
 */

// Define arrays for the anode (+) and cathode (-) wire pins.
// Your pins will probably be different.
// Remember that analog pins (A0, A1, ...) can also act as digital.
const byte CATHODE_PINS[8] = {2, 3, 4, 5, A0, A1, A2, A3};
const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};

void setup() {
  for (byte i = 0; i < 8; i++) {
    pinMode(ANODE_PINS[i], OUTPUT);
    pinMode(CATHODE_PINS[i], OUTPUT);

    // Turn all LEDs off
    digitalWrite(ANODE_PINS[i], HIGH);
    digitalWrite(CATHODE_PINS[i], HIGH); // LED turns on when cathode is LOW
  }

}

void loop() {
  for (byte i = 0; i < 8; i++) {
    
    digitalWrite(ANODE_PINS[i], LOW);
   
    for (byte j = 0; j < 8; j++) {
      digitalWrite(CATHODE_PINS[j], LOW);
      delay(100);
      digitalWrite(CATHODE_PINS[j], HIGH);
      
    }
    
   digitalWrite(ANODE_PINS[i], HIGH);
  }  
}
