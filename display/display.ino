/* LED array test code
 *
 * Reads (x,y) co-ordinates from the Serial Monitor and toggles the state of
 * the LED at that co-ordinate. The co-ordinates are specified as "x y", e.g.
 * "1 2", followed by a newline. Invalid co-ordinates are rejected.
 *
 * You need to fill in all the places marked TODO.
 *
 * == Setting up the Serial Monitor ==
 * The Serial Monitor must be configured (bottom-right corner of the screen) as:
 *   - Newline (for the line ending)
 *   - Baud rate 115200
 *
 * ENGR 40M
 * July 2018
 */

// Arrays of pin numbers. Fill these in with the pins to which you connected
// your anode (+) wires and cathode (-) wires.

const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};
const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};

void setup() {
  for (byte i = 0; i < 8; i++) {
    pinMode(ANODE_PINS[i], OUTPUT);
    pinMode(CATHODE_PINS[i], OUTPUT);

    // Turn all LEDs off
    digitalWrite(ANODE_PINS[i], HIGH);
    digitalWrite(CATHODE_PINS[i], HIGH); // LED turns on when cathode is LOW
  }
  
  Serial.begin(115200);
  Serial.setTimeout(100);
}

/* Function: display
 * -----------------
 * Runs through one multiplexing cycle of the LEDs, controlling which LEDs are
 * on.
 *
 * During this function, LEDs that should be on will be turned on momentarily,
 * one row at a time. When this function returns, all the LEDs will be off
 * again, so it needs to be called continuously for LEDs to be on.
 */
void display(byte pattern[8][8]) {
  
   for (byte i = 0; i < 8; i++) {
    
      for (byte j = 0; j < 8; j++) {  
        
         if(pattern[i][j]){
            digitalWrite(CATHODE_PINS[j], LOW);
          } else {
             digitalWrite(CATHODE_PINS[j], HIGH);
          }
          
      }
      digitalWrite(ANODE_PINS[i], LOW);
      delayMicroseconds(100);
      digitalWrite(ANODE_PINS[i], HIGH);
      
    }
}


void loop() {
  // You shouldn't need to edit this function during lab 3a.

  // use 'static' so that it retains its value between successive calls of loop()
  static byte ledOn[8][8];

  byte x = 0;
  byte y = 0;
  static char message[60];

  if (Serial.available()) {
    // Parse the values from the serial string
    x = Serial.parseInt();
    y = Serial.parseInt();

    // Check for input validity
    if (Serial.read() != '\n') {
      Serial.println("invalid input - check that line ending is set to \"Newline\"; input must be two numbers");
      return;
    }
    if (x < 0 || x > 7 || y < 0 || y > 7) {
      sprintf(message, "x = %d, y = %d -- indices out of bounds", x, y);
      Serial.println(message);
      return;
    }

    // Print to Serial Monitor to give feedback about input
    sprintf(message, "x = %d, y = %d", x, y);
    Serial.println(message);

    // Toggle the LED state
    ledOn[x][y] = !ledOn[x][y];
  }

  // This function gets called every loop
  display(ledOn);
}
