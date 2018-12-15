#define F_CPU 8000000

#include <TFT.h>
#include <SPI.h>
#include "LowPower.h"

const int SCREEN_CS = 10;
const int SCREEN_DC = 8;
const int SCREEN_RST = 9;
const int SCREEN_RESET = 4;

const int HAND_SENSOR = 2;
const int SOIL_SENSOR = A0;
const int HIDDEN_BUTTON = 3;

const int DRY = 568;
const int WET = 360;

TFT TFTscreen = TFT(SCREEN_CS, SCREEN_DC, SCREEN_RST);

void setup() {

  pinMode(HAND_SENSOR, INPUT);
  pinMode(SOIL_SENSOR, INPUT);
  pinMode(HIDDEN_BUTTON, INPUT_PULLUP);
  
  pinMode(SCREEN_RESET, OUTPUT);
  digitalWrite(SCREEN_RESET, LOW); //turn off screen on startup
  
  TFTscreen.begin();
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);


}

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void displayHiddenMessage()
{ 
  TFTscreen.background(0, 0, 0);
  TFTscreen.setTextSize(2);
  TFTscreen.stroke(147,20,255);
  TFTscreen.text("Secret message", 20, 50);
  
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(100,255,0);
  TFTscreen.text("sam", 143, 120);
}

void displayDryFlower()
{
  TFTscreen.setTextSize(2);
  TFTscreen.stroke(0,0, 155);
  TFTscreen.text("J'ai soif!", 25, 5);
  
  // Petales
  TFTscreen.stroke(0,80,80);
  TFTscreen.fill(0,70,70);
  TFTscreen.circle(TFTscreen.width()/2 + 10, 45, 5);
  TFTscreen.circle(TFTscreen.width()/2 - 7, 37, 5);
  TFTscreen.circle(TFTscreen.width()/2 - 10, 45, 5);

  // Centre
  TFTscreen.stroke(0,50,30);
  TFTscreen.fill(0,30,30);
  TFTscreen.circle(TFTscreen.width()/2, 45, 9);

  // Tige
  TFTscreen.stroke(0,20,0);
  TFTscreen.fill(0,20,0);
  int tigeWidth = 2;
  int tigeHeight = 20;
  TFTscreen.rect(TFTscreen.width()/2 - tigeWidth/2, 55, tigeWidth, tigeHeight);

  // Pot
  TFTscreen.stroke(0,0,80);
  TFTscreen.fill(0,0,50);
  int pot1Width = 25;
  int pot1Height = 8;
  TFTscreen.rect(TFTscreen.width()/2 - pot1Width/2, 55 + tigeHeight, pot1Width, pot1Height);
  int pot2Width = 20;
  int pot2Height = 15;
  TFTscreen.rect(TFTscreen.width()/2 - pot2Width/2, 55 + tigeHeight + pot1Height, pot2Width, pot2Height);
}
void displayWetFlower()
{
  TFTscreen.setTextSize(2);
  TFTscreen.stroke(147,255, 20);
  TFTscreen.text("J'ai pas soif.", 3, 5);
  
  // Petales
  TFTscreen.stroke(0,255,255);
  TFTscreen.fill(0,120,120);
  TFTscreen.circle(TFTscreen.width()/2 + 10, 45, 5);
  TFTscreen.circle(TFTscreen.width()/2 + 7, 37, 5);
  TFTscreen.circle(TFTscreen.width()/2, 34, 5);
  TFTscreen.circle(TFTscreen.width()/2 - 7, 37, 5);
  TFTscreen.circle(TFTscreen.width()/2 - 10, 45, 5);

  // Centre
  TFTscreen.stroke(0,120,0);
  TFTscreen.fill(0,50,0);
  TFTscreen.circle(TFTscreen.width()/2, 45, 9);

  // Tige
  TFTscreen.stroke(0,20,0);
  TFTscreen.fill(0,20,0);
  int tigeWidth = 2;
  int tigeHeight = 20;
  TFTscreen.rect(TFTscreen.width()/2 - tigeWidth/2, 55, tigeWidth, tigeHeight);

  // Pot
  TFTscreen.stroke(0,0,80);
  TFTscreen.fill(0,0,50);
  int pot1Width = 25;
  int pot1Height = 8;
  TFTscreen.rect(TFTscreen.width()/2 - pot1Width/2, 55 + tigeHeight, pot1Width, pot1Height);
  int pot2Width = 20;
  int pot2Height = 15;
  TFTscreen.rect(TFTscreen.width()/2 - pot2Width/2, 55 + tigeHeight + pot1Height, pot2Width, pot2Height);
}

void loop() {
  
  // Allow wake up pin to trigger interrupt on low.
  attachInterrupt(0, wakeUp, LOW);
  attachInterrupt(1, wakeUp, LOW);
  
  // Enter power down state with ADC and BOD module disabled.
  // Wake up when wake up pin is low.
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  
  // Disable external pin interrupt on wake up pin.
  detachInterrupt(0);
  detachInterrupt(1);

  
  if (digitalRead(HIDDEN_BUTTON) == LOW) {

    displayHiddenMessage();
    digitalWrite(SCREEN_RESET, HIGH);
    delay(2000);
    digitalWrite(SCREEN_RESET, LOW);
    TFTscreen.background(0, 0, 0);

  } else if (digitalRead(HAND_SENSOR) == LOW) {

    int soilSensor = analogRead(SOIL_SENSOR);
    if (soilSensor <= WET) {
      displayWetFlower();
    } else {
      displayDryFlower();
    }

    char Msg[5]="    ";
    sprintf(Msg, "%d", soilSensor);
    TFTscreen.setTextSize(1);
    TFTscreen.stroke(11,11,11);
    TFTscreen.text(Msg, 140, 120);

    digitalWrite(SCREEN_RESET, HIGH);

    while (digitalRead(HAND_SENSOR) == LOW) {
      delay(1000);
    }

    digitalWrite(SCREEN_RESET, LOW);
    TFTscreen.background(0, 0, 0);
  }
}
