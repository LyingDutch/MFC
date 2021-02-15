#include "Timer.h"

namespace Timer
{
  bool Timer::ifTimePassed(unsigned int _timePassed)
  {
    this->last_time = millis();
    if (!this->timerStarted)
    {
      this->current_time = millis();
      this->timerStarted = true;
    }

    if (this->timerStarted == true && this->last_time >= this->current_time + _timePassed)
    {
      this->timerStarted = false;
      return true;
    }
    return false;
  }
}
