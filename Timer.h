#pragma once

namespace Timer
{
  class Timer
  {
  private:
    unsigned int current_time = 0;
    unsigned int time_passed = 0;
    unsigned int last_time = 0;

  public:
    Timer() = default;
    ~Timer() = default;

    bool timerStarted = false;

    void startTimer();
    unsigned int showTimePassed();
    bool ifTimePassed(unsigned int timePassed);
    void setLoop();
    unsigned int getLoopTime();
  };
}
