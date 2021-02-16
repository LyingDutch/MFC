#include "Button.h"

namespace LDKlok
{
  bool Button::isReleased()  //unused
   {
     lastState = currentState; 
     currentState = digitalRead(pin); 
 
     if(lastState == 0 && currentState == 1) 
     {return true;} 
  
    return false;
   }

   bool Button::isPushed()
   {
     lastState = currentState; 
     currentState = digitalRead(pin); 
 
     if(lastState == 1 && currentState == 0) 
     {return true;} 
  
    return false;
   }

   bool Button::staysPushed(int _forSeconds)
   {
     _forSeconds *= 1000;
     lastState = currentState; 
     currentState = digitalRead(pin); 

     delay(1);
     if(lastState == 0 && currentState == 1 || lastState == 1 && currentState == 0 || lastState == 1 && currentState == 1) // 
     {
      this->pushTimer.current_time = millis();
      return false;
     }
     
     else if(lastState == 0 && currentState == 0) 
     {
      if(this->pushTimer.ifTimePassed(_forSeconds))
      {
        return true;
      } 
     }     
     else{return false;}
   }
}
