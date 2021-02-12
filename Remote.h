#pragma once
#include <Time.h>
#include <DS1302.h>

namespace LDKlok
{
struct Remote
{
public:
  static const unsigned long rPowerButton = 16748655;

  static const unsigned long rN0 = 16738455;
  static const unsigned long rN1 = 16724175;
  static const unsigned long rN2 = 16718055;
  static const unsigned long rN3 = 16743045;
  static const unsigned long rN4 = 16716015;
  static const unsigned long rN5 = 16726215;
  static const unsigned long rN6 = 16734885;
  static const unsigned long rN7 = 16728765;
  static const unsigned long rN8 = 16730805;
  static const unsigned long rN9 = 16732845;

  static const unsigned long rNextSegment = 16712445;
  static const unsigned long rlastSegment = 16720605;
  
  static const unsigned long rSetTime = 16761405;
  static const unsigned long rClockPage = 16769565;
  static const unsigned long rWeatherPage = 16753245;
  static const unsigned long rHomePage = 16736925;
};
}
