#pragma once
#include <LiquidCrystal.h>
#include <Adafruit_BME280.h>
#include <Time.h>
#include <DS1302.h>
#include <IRremote.h>
#include "Button.h"
#include "Timer.h"

namespace LDKlok
{
class Interface
{
private:  
//lcd pinouts
  const int RS = 7;
  const int EN = 8;
  const int D4 = 9;
  const int D5 = 10;
  const int D6 = 11;
  const int D7 = 12;
  const int BACKLIGHTPIN = 13;

//bme pinouts
  const int KSCLK = 6;
  const int KIO = 5;
  const int KCE = 4;

//infared pinout
  int IRPIN = 3;

//button pins
  const int MENUBUTTONPIN = 14;
  const int POWERBUTTONPIN = 16;
  
  float humidity;
  float temperature;

  enum Page {WEATHER = 3, CLOCK, MAIN};  

//sets a variable to hold the remote signal
  unsigned long tempSignal = 0x00000;

//holds the direction user is going in the menu
  bool goingUp = false;
  bool goingDown = false;

//So the clock knows what to display when setting the time
  bool timeBeingSet = false;
  bool yearSet = true;
  bool segmentOneSet = false;
  bool monthSet = true;
  bool segmentTwoSet = false;
  bool daySet = true;
  bool segmentThreeSet = false;
  bool hourSet = true;
  bool segmentFourSet = false;
  bool minuteSet = true;
  bool segmentFiveSet = false;

//function to make the clockPage more readable and easier to adjust
  bool checkSelection(bool& _arg1, bool& _arg2, bool& _segmentSet, Time& _t, int _selection);
  
private:  
  String dayOfWeek(const Time::Day day);  //currently not used
  
  float getHumidity();
  float getTemperature();

  //pages
  void clockPage();
  void weatherPage();
  void mainPage();
  void setTime();
  void timerPage(int _hours, int _minutes, int _seconds, bool _secondsSet); // what is displayed for the user when running setUserTime();
  void menu();
  void setUserTimer(); // Timer Page

//checks for remote signal and responds appropiatly
  void checkSignal();

//checks if backlight of lcd needs to be on or off and responds appropiatly
  void checkPowerButton();
  void checkButtons();

private:
  Adafruit_BME280 bme; 
  DS1302 klok;
  IRrecv IR;
  decode_results IRresults;
  LiquidCrystal lcd;
  Button menuButton;
  Button powerButton;
  Timer::Timer clockBlinkTimer;
  Timer::Timer userTimer;

public:  
  Interface():
    klok(DS1302(KCE, KIO, KSCLK)), IR(IRrecv(IRPIN)), lcd(LiquidCrystal(RS, EN, D4, D5, D6, D7)), menuButton(Button(MENUBUTTONPIN, 3)), powerButton(Button(POWERBUTTONPIN, 2)) 
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
