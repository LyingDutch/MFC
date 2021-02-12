#include "Interface.h"

LDKlok::Interface interface;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
interface.lcdStartup();
interface.bmeStartup();
}

void loop() {
  // put your main code here, to run repeatedly:
interface.runPage();
}
