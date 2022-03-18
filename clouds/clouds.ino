const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};
const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};
const int BRIGHTNES_RANGE = 30;

void display(byte pattern[8][8]) {

  // For every anode
   for (byte i = 0; i < 8; i++) {
    digitalWrite(ANODE_PINS[i], LOW);
    //For every possible brightness
    for (byte x = 0; x < BRIGHTNES_RANGE; x++) {
      // For every cathode
      for (byte j = 0; j < 8; j++) {  
           if(pattern[i][j] > x){ // Only turn light on for the cycle if the brightness exceeds the current threshold
              digitalWrite(CATHODE_PINS[j], LOW); //on
           }
           else {
              digitalWrite(CATHODE_PINS[j], HIGH); //off
           }
           
       }
       delayMicroseconds(30);
    }
    digitalWrite(ANODE_PINS[i], HIGH);
  }
}

void movePatternRightClouds(byte pattern[8][8]) {
  for (int y = 7; y > 0; y--) {
    for (int x = 7; x >= 0; x--) {
        pattern[x + 1][y] = pattern[x][y]; // Move LED right
        pattern[x][y] = 0; // turn LED off
    }
  }
}


void createCloud(byte pattern [8][8]) {
  if(random(0, 2)){ // Only draw a cloud half the time
    pattern[0][random(2, 5)] = random(1, (BRIGHTNES_RANGE + 1) / 2); // Assign a random byte in the row to a random brightness
  }
}

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

const int TIME_DELAY = 900;

void loop() {
  static byte pattern[8][8];
  static long nextUpdateTime = millis();

  if (millis() > nextUpdateTime) {
    movePatternRightClouds(pattern);
    createCloud(pattern);
    nextUpdateTime = millis() + TIME_DELAY;
  }
  display(pattern);
}
