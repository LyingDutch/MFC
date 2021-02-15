#pragma once
#include <Time.h>
#include <DS1302.h>

namespace LDKlok
{
struct Remote
{
public:
  static const unsigned long POWERBUTTON = 16748655;

  static const unsigned long N0 = 16738455;
  static const unsigned long N1 = 16724175;
  static const unsigned long N2 = 16718055;
  static const unsigned long N3 = 16743045;
  static const unsigned long N4 = 16716015;
  static const unsigned long N5 = 16726215;
  static const unsigned long N6 = 16734885;
  static const unsigned long N7 = 16728765;
  static const unsigned long N8 = 16730805;
  static const unsigned long N9 = 16732845;

  static const unsigned long NEXTSEGMENT = 16712445;
  static const unsigned long LASTSEGMENT = 16720605;
  
  static const unsigned long OPTIONS = 16761405;
  static const unsigned long CLOCKPAGE = 16769565;
  static const unsigned long WEATHERPAGE = 16753245;
  static const unsigned long HOMEPAGE = 16736925;
};
}
