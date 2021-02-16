#pragma once
#include "Timer.h"

namespace LDKlok
{
class Button
{
private:
    Timer::Timer pushTimer;
      
public:
    int index, lastState, currentState;
    const int pin;

public:
   Button(const int _pin):
      pin(_pin) {}
      
   Button(const int _pin, int _startingIndex):
      pin(_pin), index(_startingIndex) {}
      
   bool isReleased();
   bool isPushed();
   bool staysPushed(int _forSeconds);
 };
}
