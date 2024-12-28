// To use ArduinoGraphics APIs, please include BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "Modulino.h"

ModulinoKnob encoder;
ModulinoThermo thermo;
ArduinoLEDMatrix matrix;
ModulinoPixels pixels;
int temp;
bool isOn;
bool encoderLock;

void drawText(const char text[]) {
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText();

  matrix.endDraw();
}

void updateDisplay() {
  char number[2];
  if (isOn)
    temp = encoder.get();
  else
    encoder.set(temp);

  if (temp > 99) {          // Set limits
    encoder.set(99);
    temp = 99;
  } else if (temp < 40) {
    encoder.set(40);
    temp = 40;
  }

  if (encoder.isPressed()) {
    encoderLock = true;
  }

  if (!encoder.isPressed() && encoderLock) {
    encoderLock = false;
    isOn = !isOn;
  }

  if (isOn) {
    itoa(temp, number, 10);
    std::string s = std::string(number) + " ";
    drawText(s.c_str());
  } else {
    drawText("  ");
  }
}

void setup() {
  temp = 74;
  encoderLock = false;
  isOn = true;
  Modulino.begin();
  encoder.begin();
  thermo.begin();
  pixels.begin();
  matrix.begin();
  encoder.set(temp);
}

void loop() {
  updateDisplay(); 
  float currentTemp = (thermo.getTemperature() * (9/5)) + 35;
  int temperatureDifference = floor(temp - currentTemp);
  pixels.clear();
  if(abs(temperatureDifference) > 0 && isOn) {
    for (int i = 0; i > temperatureDifference && i > -4; i--)
      pixels.set(4-i,0,0,255,5);
    for (int i = 0; i < temperatureDifference && i < 4; i++)
      pixels.set(3-i,255,0,0,5);
  }
  pixels.show();
  delay(100);
}
