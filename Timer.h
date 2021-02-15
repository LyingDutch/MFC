#pragma once
#include <Arduino.h>

namespace Timer
{
  class Timer
  {
  private:
    unsigned int current_time = 0;
    unsigned int time_passed = 0;
    unsigned int last_time = 0;

    bool timerStarted = false;

  public:
    Timer() = default;
    ~Timer() = default;    

    bool ifTimePassed(unsigned int timePassed);
  };
}
