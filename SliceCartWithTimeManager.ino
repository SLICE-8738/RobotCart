#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 300
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

//View array spreadsheet here: https://docs.google.com/spreadsheets/d/1uPBvxobsNldO1_tUTYW7d_6D1sfJVXOsJzB6nNNpvE0/edit?usp=sharing
//Leds 128 to 170 compose the internal illuminator
//Leds 0 to 41 compose right column 1
//Leds 84 to 42 compose right column 2
//Leds 85 to 127 compose right column 3
//Leds 171 to 213 compose left column 1
//Leds 256 to 214 compose left column 2
//Leds 257 to 298 compose left column 3 

int topButtonPin = 9;
int bottomButtonPin = 8;
int minPin = A1;
int maxPin = A2;

int micPin = A0;
int micVolume = 0;

//Method Switcher Variables
int rows = 0;
int sequence = 0;
int methods = 6;

//FastLoop Variables
int time = 0;
int sequenceStartTime = 0;
int timeInSequence = 0;
int currentMethodID = 0;
bool finished = false;
int groupPhase = 0;

void setup() { 
  Serial.begin(2000000);
  pinMode(LED_BUILTIN, OUTPUT);
  FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  //Choose HTML color codes to use here: https://g.co/kgs/S9Anr7
  //Run the method of your choice below

  micVolume = analogRead(micPin);
  time = millis();
  timeInSequence = time - sequenceStartTime;

  if(digitalRead(topButtonPin) == 1) {
    sequence++;
    while(digitalRead(topButtonPin) == 1) {
      delay(1);
    } // Wait for button to be released to prevent skipping
    Serial.println(sequence);
  } else if(digitalRead(bottomButtonPin) == 1) {
    sequence--;
    while(digitalRead(bottomButtonPin) == 1) {
      delay(1);
    } // Wait for button to be released to prevent skipping
    Serial.println(sequence);
  }

  if(sequence < 0) {
    sequence = methods - 1;
  } else if(sequence >= methods) {
    sequence = 0;
  }

  switch(sequence) {
    case 0:
      clearLeds(true);
      break;
    case 1:
      rainbow(3);
      break;
    case 2:
      noise(micVolume, analogRead(maxPin), analogRead(minPin));
      break;
    case 3:
      blink(255, 50, 0);
      break;
    case 4:
      cycleAllLetters(255, 50, 0, 2);
      break;
    case 5:
      cycleAndBlink();
      break;
    default:
      clearLeds(true);
      break;
  }

}

//
// End setup
//
// Begin sequences
//

void blink(int red, int green, int blue) {
  // Initialize function at 0 ticks
  if(currentMethodID != 1111) {
    currentMethodID = 1111;
    sequenceStartTime = time;
    finished = false;
    return;
  }

  // Show color for 500 ticks
  if(timeInSequence < 500) {
    setAll(red, green, blue);
    return;
  }

  // Show black for next 500 ticks
  if(timeInSequence < 1000) {
    setAll(0, 0, 0);
    return;
  }

  restart();
}

void rainbow(int speed) {
  if(currentMethodID != 1112) {
    currentMethodID = 1112;
    sequenceStartTime = time;
    finished = false;
    return;
  }

  // Red
  if(timeInSequence < 142*speed) {
    setAll(255, 0, 0);
    return;
  }

  // Orange
  if(timeInSequence < 142*speed*2) {
    setAll(255, 50, 0);
    return;
  }

  // Yellow
  if(timeInSequence < 142*speed*3) {
    setAll(255, 200, 0);
    return;
  }

  // Green
  if(timeInSequence < 142*speed*4) {
    setAll(0, 255, 0);
    return;
  }

  // Blue
  if(timeInSequence < 142*speed*5) {
    setAll(0, 0, 255);
    return;
  }

  // Purple
  if(timeInSequence < 142*speed*6) {
    setAll(128, 0, 255);
    return;
  }

  restart();
}

void noise(int volume, int max, int min) {
  if(currentMethodID != 1113) {
    currentMethodID = 1113;
    sequenceStartTime = time;
    finished = false;
    return;
  }

  volume -= min/4;
  if(volume < 0) {
    volume = 0;
  }
  volume = volume * (255 / ((255 - max/4) - min/4));

  int updatedRows = round(volume / 5.93);

  if(updatedRows > rows) {
    rows = updatedRows;
  }

  if(rows > 41) {
    rows = 41;
  }

  clearLeds(false);

  for (int i = 0; i < rows; i++) {
    setRow(i, 255, 50, 0);
  }

  updateLeds();

  if(rows > 0) {
    rows--;
  }
}

void cycleAllLetters(int red, int green, int blue, int speed) {
  if(currentMethodID != 1114) {
    currentMethodID = 1114;
    sequenceStartTime = time;
    finished = false;
    return;
  }

  clearLeds(false);

  // S
  if(timeInSequence < 142*speed) {
    setLetter(1, red, green, blue);
    return;
  }

  // L
  if(timeInSequence < 142*speed*2) {
    setLetter(2, red, green, blue);
    return;
  }

  // I
  if(timeInSequence < 142*speed*3) {
    setLetter(3, red, green, blue);
    return;
  }

  // C
  if(timeInSequence < 142*speed*4) {
    setLetter(4, red, green, blue);
    return;
  }

  // E
  if(timeInSequence < 142*speed*5) {
    setLetter(5, red, green, blue);
    return;
  }

  restart();
}

//
// End sequences
//
// Begin groups
//

void cycleAndBlink() {
  if(groupPhase == 0) {
    finished = false;
    groupPhase++;
  }

  if(finished == true) {
    if(groupPhase >= 4) {
      groupPhase = 0;
      return;
    } else {
      groupPhase++;
      finished = false;
    }
  }

  if(finished == false && groupPhase == 1) {
    cycleAllLetters(255, 50, 0, 2);  
  }

  if(finished == false && groupPhase > 1) {
    blink(255, 50, 0); 
  }
}

//
// End groups
// 
// Begin setters
//

void setLetter(int letter, int red, int green, int blue) {
  if(letter == 1) {
    for(int i = 34; i < 43; i++) {
      setRow(i, red, green, blue);
    }
  } else if(letter == 2) {
    for(int i = 26; i < 34; i++) {
      setRow(i, red, green, blue);
    }
  } else if(letter == 3) {
    for(int i = 17; i < 26; i++) {
      setRow(i, red, green, blue);
    }
  } else if(letter == 4) {
    for(int i = 8; i < 17; i++) {
      setRow(i, red, green, blue);
    }
  } else if(letter == 5) {
    for(int i = 0; i < 8; i++) {
      setRow(i, red, green, blue);
    }
  }

  updateLeds();  
}

// Update a row of lights from 0 (E1) to 42 (S9)
/// @param colors the rgb value to set to
void setRow(int row, int red, int green, int blue) {
  //right
  leds[row].setRGB(red, green, blue);
  leds[84 - row].setRGB(red, green, blue);
  leds[85 + row].setRGB(red, green, blue);

  //left
  leds[171 + row].setRGB(red, green, blue);
  leds[256 - row].setRGB(red, green, blue);
  leds[257 + row].setRGB(red, green, blue);
}

// Sets all the LEDs to the provided color
/// @param colors the rgb value to set to
void setAll(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(red, green, blue);
  }

  updateLeds();
}

//Clears out the leds, providing the option of displaying it
void clearLeds(bool update) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }

  if(update) {
    updateLeds();
  }
}

// Display the LED states
void updateLeds() {
  for (int i = 128; i <= 170; i++) {
    leds[i].setRGB(255, 255, 255);
  }

  FastLED.setBrightness(100);
  FastLED.show();
}

void restart() {
  sequenceStartTime = time;
  finished = true;
}

//
// End setters
//