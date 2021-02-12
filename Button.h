#pragma once

namespace LDKlok
{
class Button
{
public:
    int index, lastState, currentState;
    const int pin;

public:
   Button(const int _pin, int _index):
      pin(_pin), index(_index) {}
      
   bool isReleased()
   {
     lastState = currentState; 
     currentState = digitalRead(pin); 
 
     if(lastState == 0 && currentState == 1) 
     {return true;} 
  
    return false;
   }

   bool isPushed()
   {
     lastState = currentState; 
     currentState = digitalRead(pin); 
 
     if(lastState == 1 && currentState == 0) 
     {return true;} 
  
    return false;
   }

   bool staysPushed()
   {
     lastState = currentState; 
     currentState = digitalRead(pin); 
 
     if(lastState == 0 && currentState == 0) 
     {return true;} 
  
      return false;
   }
};
}
