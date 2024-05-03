#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef PSTR
#define PSTR  // Make Arduino Due happy
#endif

// How many leds in your strip?
#define NUM_LEDS 300
#define DATA_PIN 3

#define GRID_NUM_ROWS 8
#define GRID_NUM_COLS 32
#define GRID_TILES_X 2
#define GRID_TILES_Y 2

#define DISPLAY_PIN 7

#define CHAR_PIX_WIDTH 6
#define TOP_BUTTON_PIN 9
#define BOTTOM_BUTTON_PIN 8
#define MIN_PIN A1
#define MAX_PIN A2
#define LED_PROGRAMS 8

#define MIC_PIN A0

/**
 * Defined a group of variables focused on rolling through the colorwheel
 * at a given speed. Allows you to create a colorwheel then spin it at a given speed
 * which allows you to get red, green and blue values out for that color
 **/
class ColorWheel {
  public:
    // Red green and blue
    int red = 0;
    int green = 0;
    int blue = 0;

	  ColorWheel() {}

    void resetRainbow() {
      state = 0;
      red = 255;
      green = 0;
      blue = 0;
      red_done = true;
      green_done = false;
      blue_done = false;
    }

    void resetRainbow(int colorsIn){
      for (int i = 0; i < colorsIn; i++)
        rollingRainbow(1);
    }

    void rollingRainbow(int jump) {
      switch (state) {
        case 0:
          if (green == 255) {
            state = 1;
          } else {
            green = green + jump;
          }
          break;
        case 1:
          if (red == 0) {
            state = 2;
          } else {
            red = red - jump;
          }
          break;
        case 2:
          if (blue == 255) {
            state = 3;
          } else {
            blue = blue + jump;
          }
          break;
        case 3:
          if (green == 0) {
            state = 4;
          } else {
            green = green - jump;
          }
          break;
        case 4:
          if (red == 255) {
            state = 5;
          } else {
            red = red + jump;
          }
          break;
        case 5:
          if (blue == 0) {
            state = 0;
          } else {
            blue = blue - jump;
          }
          break;
      }

      // Handle multiples smaller than things that add up to 0 or 255
      if (red > 255){
        red = 255;
      }
      if (green > 255){
        green = 255;
      }
      if (blue > 255){
        blue = 255;
      }
      if (red < 0){
        red = 0;
      }
      if (green < 0){
        green = 0;
      }
      if (blue < 0){
        blue = 0;
      }
    }
  
  private:
    int state = 0;
    bool red_done = false;
    bool green_done = false;
    bool blue_done = false;
};

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
/*Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(GRID_NUM_COLS, GRID_NUM_ROWS, DISPLAY_PIN,
                                               NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

*/
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(GRID_NUM_COLS, GRID_NUM_ROWS, GRID_TILES_X, GRID_TILES_Y, DISPLAY_PIN,
                                               NEO_TILE_TOP + NEO_TILE_RIGHT + NEO_TILE_ROWS + NEO_TILE_ZIGZAG +
                                               NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

const uint16_t colors[] = {
  matrix.Color(255, 0, 0),
  matrix.Color(255, 127, 0),
  matrix.Color(255, 255, 0),
  matrix.Color(127, 255, 0),
  matrix.Color(0, 255, 0),
  matrix.Color(0, 255, 127),
  matrix.Color(0, 255, 255),
  matrix.Color(0, 127, 255),
  matrix.Color(0, 0, 255),
  matrix.Color(127, 0, 255),
  matrix.Color(255, 0, 255),
  matrix.Color(255, 0, 127)
};

const uint16_t ORANGE = matrix.Color(255, 127, 0);
const uint16_t WHITE = matrix.Color(255, 255, 255);
const uint16_t BLACK = matrix.Color(0, 0, 0);

ColorWheel sideColorWheel = ColorWheel();
ColorWheel matrixColorWheel = ColorWheel();

int matrixWidth = matrix.width();

int endCounter;

void resetRainbowScroll() {
  matrixWidth = matrix.width();
  sideColorWheel.resetRainbow();
  matrixColorWheel.resetRainbow();
}

//View array spreadsheet here: https://docs.google.com/spreadsheets/d/1uPBvxobsNldO1_tUTYW7d_6D1sfJVXOsJzB6nNNpvE0/edit?usp=sharing
//Leds 128 to 170 compose the internal illuminator
//Leds 0 to 41 compose right column 1
//Leds 84 to 42 compose right column 2
//Leds 85 to 127 compose right column 3
//Leds 171 to 213 compose left column 1
//Leds 256 to 214 compose left column 2
//Leds 257 to 298 compose left column 3

int micVolume = 0;

//Method Switcher Variables
int rows = 0;
int sequence = 0;

//FastLoop Variables
int time = 0;

int sideSequenceStartTime = 0;
String currentSideMethod = "";
int timeInSideSequence = 0;
bool sideFinished = false;

int scrollSequenceStartTime = 0;
String currentScrollMethod = "";
int timeInScrollSequence = 0;
bool scrollFinished = false;

int groupPhase = 0;

void setup() {
  Serial.begin(500000);
  pinMode(LED_BUILTIN, OUTPUT);
  strip.begin();
  strip.show();

  matrix.begin();
  matrix.show();
  matrix.setTextWrap(false);
  matrix.setBrightness(100);
  matrix.setTextColor(ORANGE);
  matrix.setTextSize(2);
  resetRainbowScroll();
}

void loop() {
  //Choose HTML color codes to use here: https://g.co/kgs/S9Anr7
  //Run the method of your choice below
  micVolume = analogRead(MIC_PIN);
  time = millis();
  timeInSideSequence = time - sideSequenceStartTime;
  timeInScrollSequence = time - scrollSequenceStartTime;

  // Don't let it overflow
  if (timeInSideSequence > 32000){
    timeInSideSequence = 0;
  }
  if (timeInScrollSequence > 32000){
    timeInScrollSequence = 0;
  }


  if (digitalRead(TOP_BUTTON_PIN) == 1) {
    sequence++;
    while (digitalRead(TOP_BUTTON_PIN) == 1) {
      delay(1);
    }  // Wait for button to be released to prevent skipping
    if (sequence < 0)
      sequence = LED_PROGRAMS - 1;
    else if (sequence >= LED_PROGRAMS)
      sequence = 0;
      
    Serial.print("Sequence: ");
    Serial.println(sequence);
  } else if (digitalRead(BOTTOM_BUTTON_PIN) == 1) {
    sequence--;
    while (digitalRead(BOTTOM_BUTTON_PIN) == 1) {
      delay(1);
    }  // Wait for button to be released to prevent skipping
    if (sequence < 0)
      sequence = LED_PROGRAMS - 1;
    else if (sequence >= LED_PROGRAMS)
      sequence = 0;
    
    Serial.print("Sequence: ");
    Serial.println(sequence);
  }


  switch (sequence) {
    case 0:
      resetRainbowScroll();
      strip.clear();
      strip.show();
      matrix.clear();
      matrix.show();
      break;
    case 1:
      rainbow(15);
      rainbowScrollingText(F("SLICE! SLICE! SQUEEZE!!"), 2, 15, 2, false);
      break;
    case 2:
      rainbow(15);
      rainbowScrollingText(F("SLICE! SLICE! SQUEEZE!!"), 2, 85, 2, true);
      break;
    case 3:
      noise(micVolume, analogRead(MAX_PIN), analogRead(MIN_PIN));
      alternatePhrases("SLICE", "8738", colors[0], 800, 300);
      break;
    case 4:
      blink(255, 50, 0);
      alternatePhrases("SLICE", "8738", ORANGE, 3000, 0, true);
      break;
    case 5:
      cycleAllLetters(255, 50, 0, 2, true);
      break;
    case 6:
      cycleAndBlink();
      coloredScrollingText(F("SLICE! SLICE! SQUEEZE!!"), 2, 2, ORANGE);
      break;
    case 7:
      verticalRainbow();
      break;
    default:
      matrix.clear();
      strip.clear();
      matrix.show();
      strip.show();
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
  if (!currentSideMethod.equals("blink")) {
    currentSideMethod = "blink";
    sideSequenceStartTime = time;
    sideFinished = false;
  }

  // Show color for 500 ticks
  if (timeInSideSequence < 500) {
    setAll(red, green, blue);
    return;
  }

  // Show black for next 500 ticks
  if (timeInSideSequence < 1000) {
    setAll(0, 0, 0);
    return;
  }

  restartSide();
}

void rainbow(int colorSpeed) {
  if (!currentSideMethod.equals("rainbow")) {
    currentSideMethod = "rainbow";
    sideSequenceStartTime = time;
    sideFinished = false;
    clearMatrix();
  }

  setAll(sideColorWheel.red, sideColorWheel.green, sideColorWheel.blue);
  sideColorWheel.rollingRainbow(colorSpeed);

  restartSide();
}

void noise(int volume, int max, int min) {
  if (!currentSideMethod.equals("noise")) {
    currentSideMethod = "noise";
    sideSequenceStartTime = time;
    sideFinished = false;
    clearMatrix();
  }

  volume -= min / 4;
  if (volume < 0) {
    volume = 0;
  }
  volume = volume * (255 / ((255 - max / 4) - min / 4));

  int updatedRows = round(volume / 5.93);

  if (updatedRows > rows) {
    rows = updatedRows;
  }

  if (rows > 41) {
    rows = 41;
  }

  strip.clear();

  for (int i = 0; i < rows; i++) {
    setRow(i, 255, 50, 0);
  }

  updateLeds();

  if (rows > 0) {
    rows--;
  }
}

int cycleCount = 0;

void verticalRainbow() {
  if (!currentSideMethod.equals("verticalRainbow")) {
    currentSideMethod = "verticalRainbow";
    sideSequenceStartTime = time;
    sideFinished = false;
    cycleCount = 0;
    strip.clear();
    clearMatrix();
    matrixColorWheel.resetRainbow();
  } else {
    int rainbowStep = 15;
    cycleCount++;

    matrixColorWheel.resetRainbow();
    // Jump forwarward to where the rainbow stopped plus one
    matrixColorWheel.resetRainbow((cycleCount * (6 + matrix.width())) + rainbowStep);

    for (int l = 1; l <= 3; l++){
      setColumn(l, matrixColorWheel.red, matrixColorWheel.green, matrixColorWheel.blue);
      matrixColorWheel.rollingRainbow(rainbowStep);
    }

    for (int i = 0; i < matrix.width(); i++){
        //matrix.drawLine(i, 0, i, 16, matrix.Color(red, green, blue));
        matrix.drawFastVLine(i, 0, 16, matrix.Color(matrixColorWheel.red, matrixColorWheel.green, matrixColorWheel.blue));
        matrixColorWheel.rollingRainbow(rainbowStep);
    }

    for (int l = 4; l <= 6; l++){
      setColumn(l, matrixColorWheel.red, matrixColorWheel.green, matrixColorWheel.blue);
      matrixColorWheel.rollingRainbow(rainbowStep);
    }

    // Switch back and forth every 2 seconds
    if (timeInSideSequence % 4000 < 2000){
      showText("8738", BLACK, false, false);
    } else {
      showText("SLICE", BLACK, false, false);
    }

    matrix.show();
    strip.show();
  }

}


void cycleAllLetters(int red, int green, int blue, int speed, boolean includeMatrix) {
  if (!currentSideMethod.equals("cycleAllLetters")) {
    currentSideMethod = "cycleAllLetters";
    sideSequenceStartTime = time;
    sideFinished = false;
    clearMatrix();
  }

  strip.clear();

  // S
  if (timeInSideSequence < 142 * speed) {
    restartScroll();
    setLetter(1, red, green, blue);
    if (includeMatrix) showText("SLICE", colors[1]);
    return;
  }

  // L
  if (timeInSideSequence < 142 * speed * 2) {
    setLetter(2, red, green, blue);
    if (includeMatrix) showText("SLICE", WHITE, true, true);
    return;
  }

  // I
  if (timeInSideSequence < 142 * speed * 3) {
    setLetter(3, red, green, blue);
    if (includeMatrix) coloredScrollingText(F("SQUEEZE!"), 0, 3, colors[3]);
    return;
  }

  // C
  if (timeInSideSequence < 142 * speed * 4) {
    setLetter(4, red, green, blue);
    if (includeMatrix) coloredScrollingText(F("SQUEEZE!"), 0, 3, colors[3]);
    return;
  }

  // E
  if (timeInSideSequence < 142 * speed * 5) {
    setLetter(5, red, green, blue);   
    if (includeMatrix) coloredScrollingText(F("SQUEEZE!"), 0, 3, colors[3]);
    return;
  }

  restartSide();
}

//
// End sequences
//
// Begin groups
//

void cycleAndBlink() {
  if (groupPhase == 0) {
    sideFinished = false;
    groupPhase++;
  }

  if (sideFinished == true) {
    if (groupPhase >= 4) {
      groupPhase = 0;
      return;
    } else {
      groupPhase++;
      sideFinished = false;
    }
  }

  if (sideFinished == false && groupPhase == 1) {
    cycleAllLetters(255, 50, 0, 2, false);
  }

  if (sideFinished == false && groupPhase > 1) {
    blink(255, 50, 0);
  }
}

//
// End groups
//
// Begin setters
//

void setLetter(int letter, int red, int green, int blue) {
  if (letter == 1) {
    for (int i = 34; i < 43; i++) {
      setRow(i, red, green, blue);
    }
  } else if (letter == 2) {
    for (int i = 26; i < 34; i++) {
      setRow(i, red, green, blue);
    }
  } else if (letter == 3) {
    for (int i = 17; i < 26; i++) {
      setRow(i, red, green, blue);
    }
  } else if (letter == 4) {
    for (int i = 8; i < 17; i++) {
      setRow(i, red, green, blue);
    }
  } else if (letter == 5) {
    for (int i = 0; i < 8; i++) {
      setRow(i, red, green, blue);
    }
  }

  updateLeds();
}

// Update a row of lights from 0 (E1) to 42 (S9)
/// @param colors the rgb value to set to
void setRow(int row, int red, int green, int blue) {
  //right
  strip.setPixelColor(row, red, green, blue);
  strip.setPixelColor(84 - row, red, green, blue);
  strip.setPixelColor(85 + row, red, green, blue);

  //left
  strip.setPixelColor(171 + row, red, green, blue);
  strip.setPixelColor(256 - row, red, green, blue);
  strip.setPixelColor(257 + row, red, green, blue);
}

// Update a row of lights from 0 (E1) to 42 (S9)
/// @param colors the rgb value to set to
void setColumn(int col, int red, int green, int blue) {
  switch (col) {
    case 1:
      updateLEDs(0, 41, red, green, blue);
      break;
    case 2:
      updateLEDs(42, 84, red, green, blue);
      break;
    case 3:
      updateLEDs(85, 127, red, green, blue);
      break;
    case 4:
      updateLEDs(171, 213, red, green, blue);
      break;
    case 5:
      updateLEDs(214, 256, red, green, blue);
      break;
    case 6:
      updateLEDs(257, 298, red, green, blue);
      break;
  }
}

void updateLEDs(int start, int end, int red, int green, int blue) {
  for (int i = start; i <= end; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
}

// Sets all the LEDs to the provided color
/// @param colors the rgb value to set to
void setAll(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, red, green, blue);
  }

  updateLeds();
}

// Display the LED states
void updateLeds() {
  for (int i = 128; i <= 170; i++) {
    strip.setPixelColor(i, 255, 255, 255);
  }

  strip.setBrightness(100);
  strip.show();
}

void restartSide() {
  sideSequenceStartTime = time;
  sideFinished = true;
}

void restartScroll(){
  scrollSequenceStartTime = time;
  scrollFinished = true;
  matrixWidth = matrix.width();
}

bool alternate = false;

void alternatePhrases(String text, String altText, uint16_t color, int displayTime, int pauseBuffer){
  alternatePhrases(text, altText, color, displayTime, pauseBuffer, false);
}

void alternatePhrases(String text, String altText, uint16_t color, int displayTime, int pauseBuffer, bool inverted){
  if (!currentScrollMethod.equals("alternatePhrases")) {
    currentScrollMethod = "alternatePhrases";
    scrollSequenceStartTime = time;
    scrollFinished = false;
  }

  int startTime = 0;
  int endTimeText = displayTime;
  int endTimePause = endTimeText + pauseBuffer;
  int endTimeAltText = endTimePause + displayTime;
  int endTime = endTimeAltText + pauseBuffer;

  alternate = timeInScrollSequence >= endTimePause;

  // Give .2 seconds clear between words
  if ((timeInScrollSequence > endTimeText && timeInScrollSequence <= endTimePause) 
      || (timeInScrollSequence > endTimeAltText && timeInScrollSequence <= endTime)) 
  {
    // Show blank
    clearMatrix();
  } else if (timeInScrollSequence > endTime){
    // Start over
    restartScroll();
  } else {
    // Show text
    String output = text;
    if (alternate){
      output = altText;
    }
    showText(output, color, true, inverted);
  }
}

void showText(String text, uint16_t color){
  showText(text, color, true, false);
}

void showText(String text, uint16_t color, bool clear, bool inverted){
  if (clear) matrix.clear();
  
  if (inverted){
    matrix.fillScreen(color);
    matrix.setTextColor(BLACK);
  } else {
    matrix.setTextColor(color);
  }

  int padding = (int)(((GRID_NUM_COLS * GRID_TILES_X) - ((int)text.length() * CHAR_PIX_WIDTH * 2))/2) + 1;
  matrix.setCursor(padding, 1);
  matrix.print(text);
  matrix.show();

}

void clearMatrix(){
  matrix.clear();
  matrix.show();
}

void coloredScrollingText(String scrollText, int pauseTime, int moveDelta, uint16_t color){
  if (!currentScrollMethod.equals("coloredScrollingText")) {
    currentScrollMethod = "coloredScrollingText";
    scrollSequenceStartTime = time;
    scrollFinished = false;
    clearMatrix();
    resetRainbowScroll();
  }

  matrix.setTextColor(color);
  scrollingText(scrollText, pauseTime, moveDelta);
}

void rainbowScrollingText(String scrollText, int pauseTime, int colorSpeed, int moveDelta, bool rainbowText){
  if (!currentScrollMethod.equals("rainbowScrollingText")) {
    currentScrollMethod = "rainbowScrollingText";
    scrollSequenceStartTime = time;
    scrollFinished = false;
    clearMatrix();
    resetRainbowScroll();
  }  

  if (!rainbowText){
    matrix.setTextColor(matrix.Color(matrixColorWheel.red, matrixColorWheel.green, matrixColorWheel.blue));
    matrixColorWheel.rollingRainbow(colorSpeed);
  }
  scrollingText(scrollText, pauseTime, moveDelta, rainbowText, colorSpeed);
}

void scrollingText(String scrollText, int pauseTime, int moveDelta) {
  scrollingText(scrollText, pauseTime, moveDelta, false, 0);
}

void scrollingText(String scrollText, int pauseTime, int moveDelta, bool rainbow, int colorSpeed) {
  matrix.clear();

  matrix.setCursor(matrixWidth, 1);
  if (rainbow){
    matrixColorWheel.resetRainbow();
    for (int i = 0; i < scrollText.length(); i++){
      matrixColorWheel.rollingRainbow(colorSpeed);
      matrix.setTextColor(matrix.Color(matrixColorWheel.red, matrixColorWheel.green, matrixColorWheel.blue));
      matrix.print(scrollText.charAt(i));
      Serial.print(scrollText.charAt(i));
    }
  } else {
    matrix.print(scrollText);
  }
  Serial.println("here");

  int length = (int)scrollText.length() * CHAR_PIX_WIDTH * 2;
  matrixWidth = matrixWidth - moveDelta;
  if (matrixWidth < -length) {
    restartScroll();
  }
  matrix.show();
  delay(pauseTime);
}
