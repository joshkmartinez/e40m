#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi.h> // Found these libraries online and included them in order to be able to make network requests.

char ssid[] = "Joshuas_iPhone";
char pass[] = "password123";

String apiKey = SECRET_API_KEY;
String location = "stanford";

int status = WL_IDLE_STATUS;
char server[] = "api.openweathermap.org";    

WiFiClient client;

const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};
const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};
const int BRIGHTNES_RANGE = 30; // Number of possible brightness values
String WEATHER_CONDITION = ""; // Variable that will store the fetched weather condition, start it as empty.
// If the weather failes to get fetched, the sun animation plays by default.

// Definition of the sun position for the sun animation to avoid moving the sun along with its rays.
const byte sun[8][8] = {
  {0, 0, 0, 0, 0, BRIGHTNES_RANGE, BRIGHTNES_RANGE, BRIGHTNES_RANGE},
  {0, 0, 0, 0, 0, BRIGHTNES_RANGE, BRIGHTNES_RANGE, BRIGHTNES_RANGE},
  {0, 0, 0, 0, 0, BRIGHTNES_RANGE, BRIGHTNES_RANGE, BRIGHTNES_RANGE},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

/*
 * Helper function to display any pattern on the display at any brightness.
 */
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
       delayMicroseconds(10);
    }
    digitalWrite(ANODE_PINS[i], HIGH);
  }
}

/*
 * Move all sun rays to the botttom right while leaving the sun in the top left.
 */
void movePatternCornerSun(byte pattern[8][8]) {
  for (byte y = 0; y < 8; y++) {
    for (byte x = 0; x < 8; x++) {
      if(pattern[x][y] > sun[x][y]){ // Only move if greater than the brightness than what's defined in the sun pattern.
        pattern[x + 1][y - 1] = pattern[x][y]; // Move LED down and to the right
        pattern[x][y] = 0; // turn LED off
      }
    }
  }
}


/*
 * Randomly create sun rays anywhere around the sun.
 */
void createRandomSunRay(byte pattern [8][8]) {
  pattern[random(0, 4)][random(4, 8)] = random(1, BRIGHTNES_RANGE + 1);
}

/*
 * Move the clouds to the right of the screen.
 */
void movePatternRightClouds(byte pattern[8][8]) {
  for (int y = 7; y > 0; y--) {
    for (int x = 7; x >= 0; x--) { // Looping in reverse order to avoid moving a cloud infinitely to the edge.
        pattern[x + 1][y] = pattern[x][y]; // Move LED right
        pattern[x][y] = 0; // turn LED off
    }
  }
}

/*
 * Randomly draw clouds in the middle of the screeen.
 */
void createCloud(byte pattern [8][8]) {
  if(random(0, 2)){ // Only draw a cloud half the time
    pattern[0][random(2, 5)] = random(1, (BRIGHTNES_RANGE + 1) / 2); // Assign a random byte in the row to a random brightness
  }
}

/*
 * Move rain down the LEDs, create lighter rain splashes once they reach the bottom.
 */
void movePatternDownRain(byte pattern[8][8]) {
  for (byte y = 1; y < 8; y++) {
    for (byte x = 0; x < 8; x++) {
      pattern[x][y - 1] = pattern[x][y]; // Move LED down
      pattern[x][y] = 0; // turn LED off

      //Splashes
      if(y == 1 && pattern[x][y - 1]){
        pattern[x-1][y+1] = pattern[x][y - 1]/3;
        pattern[x+1][y+1] = pattern[x][y - 1]/3;
      }
    }
  }
}

/*
 * Create rain droplets of random brightness in the top row.
 */
void chooseRandomInTopRowRain(byte pattern [8][8]) {
  pattern[random(0, 8)][7] = random(0, BRIGHTNES_RANGE + 1); // Assign a random byte in the row to a random brightness
}

/*
 * Fetch weather, return the main weather condition.
 */
String fetchWeatherConditions() {
  if (client.connect(server, 80)) {
    client.print("GET /data/2.5/weather?");
    client.print("q="+location);
    client.print("&appid="+apiKey);
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Connection Error");
  }

  delay(1000);
  String line = "";

  while (client.connected()) {
    line = client.readStringUntil('\n');

    StaticJsonBuffer<5000> jsonBuffer; // Create a JSON object so I can parse the weather data

    JsonObject& root = jsonBuffer.parseObject(line);
    if (!root.success()) {
      Serial.println("Failed to parse output as JSON");
      return;
    }

    return root["weather"]["main"];
  }
}

/*
 * On setup, connect to the wifi network, fetch the weather, and turn all LEDs off.
 */
void setup() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);
    delay(1000);
  }
  Serial.println("Connected to wifi");

  WEATHER_CONDITION = fetchWeatherConditions()
  
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

const int TIME_DELAY = 300;

/*
 * Based off of the weather, render the corresponding animation.
 */
void loop() {
  static byte pattern[8][8];
  static long nextUpdateTime = millis();

  if (millis() > nextUpdateTime) {
    if(WEATHER_CONDITION = "Clouds"){
      movePatternRightClouds(pattern);
      createCloud(pattern);
    }
    if(WEATHER_CONDITION = "Rain"){
      movePatternDownRain(pattern);
      chooseRandomInTopRowRain(pattern);
    } else { // Sunny is a catch all for all other weather states
      movePatternCornerSun(pattern);
      createRandomSunRay(pattern);
    }
    nextUpdateTime = millis() + TIME_DELAY;
  }
  
  display(pattern);
}
