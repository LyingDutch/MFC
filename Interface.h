#pragma once
#include <LiquidCrystal.h>
#include <Adafruit_BME280.h>
#include <Time.h>
#include <DS1302.h>
#include <IRremote.h>
#include "Button.h"

namespace LDKlok
{
class Interface
{
private:  
//lcd pinouts
  const int rs = 7;
  const int en = 8;
  const int d4 = 9;
  const int d5 = 10;
  const int d6 = 11;
  const int d7 = 12;
  const int bLight = 13;

//bme pinouts
  const int kSclk = 6;
  const int kIo = 5;
  const int kCe = 4;

//infared pinout
  int IRpin = 3;

//button pins
  const int menuButtonPin = 14;
  const int powerButtonPin = 16;
  
  float humidity;
  float temperature;

  enum Page {WEATHER = 3, CLOCK, MAIN};  

//sets a variable to hold the remote signal
  unsigned long tempSignal = 0x00000;

  bool timeBeingSet = false;
  bool yearSet = true;
  bool monthSet = true;
  bool daySet = true;
  bool hourSet = true;
  bool minuteSet = true;
  
private:  
  String dayOfWeek(const Time::Day day);  //currently not used
  
  float getHumidity();
  float getTemperature();
  
  void clockPage();
  void weatherPage();
  void mainPage();
  void setTime();

//checks for remote signal and responds appropiatly
  void checkSignal();

//checks if backlight of lcd needs to be on or off and responds appropiatly
  void checkButtons();
  void checkPowerButton();

private:
  Adafruit_BME280 bme; 
  DS1302 klok;
  IRrecv IR;
  decode_results IRresults;
  LiquidCrystal lcd;
  Button menuButton;
  Button powerButton;

public:  
  Interface():
    klok(DS1302(kCe, kIo, kSclk)), IR(IRrecv(IRpin)), lcd(LiquidCrystal(rs, en, d4, d5, d6, d7)), menuButton(Button(menuButtonPin, 3)), powerButton(Button(powerButtonPin, 2)) 
    {
      /*Time t(2021, 2, 11, 21, 16, 20, 4);
      klok.time(t);                             // Set Time Trough Code 
      */
      
      klok.halt(false);
      klok.writeProtect(false);
    }
  
  void lcdStartup();
  void bmeStartup(); 
  void runPage();
};
}
