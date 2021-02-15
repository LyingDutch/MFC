#include "Interface.h"
#include "Remote.h"

namespace LDKlok
{
//startup functions (only run once)
void Interface::lcdStartup()
{
  this->IR.enableIRIn();  // Enables the Infared
  pinMode(this->BACKLIGHTPIN, OUTPUT);
  this->lcd.begin(16, 2);
  digitalWrite(this->BACKLIGHTPIN, HIGH);
  this->lcd.setCursor(0, 0);
  this->lcd.print("Verbinding LCD");
  delay(1000);  
}

void Interface::bmeStartup()
{
  bool _status;

  _status = this->bme.begin(0x76);
  if(!_status)
  {
    this->lcd.clear();
    this->lcd.print("BME Failure");
    delay(1200);
  }

  else
  {
    this->lcd.clear(); 
    this->lcd.print("BME Connected");
    delay(1200);
  }
}


//pages setup
float Interface::getHumidity()
{this->humidity = this->bme.readHumidity();}

float Interface::getTemperature()
{this->temperature = this->bme.readTemperature()-1,3;}

void Interface::clockPage()
{
  Time _t = this->klok.time();
  this->lcd.clear();
  
  if(this->menuButton.index % 3 == 1 && !this->timeBeingSet)
  {
    this->lcd.setCursor(0, 0); 
    this->lcd.print("X");
  }
  this->lcd.setCursor(3, 0);
  if (this->daySet)
  {
    if(_t.date < 10) {this->lcd.print("0");}
    this->lcd.print(_t.date);
  }

  else 
  {
    if(this->clockBlinkTimer.ifTimePassed(500))
    {
    this->lcd.print("00");  
    delay(80);
    }
  }
  
  this->lcd.setCursor(5, 0);
  this->lcd.print("-");
  if (this->monthSet)
  {
    if(_t.mon < 10) {this->lcd.print("0");}
    this->lcd.print(_t.mon);
  }
  this->lcd.setCursor(8, 0);
  this->lcd.print("-");
  if (this->yearSet)
  {
    this->lcd.print(_t.yr);
  }
  else 
  {
    this->lcd.print("20");
  }
  this->lcd.setCursor(6, 1);
  if (this->hourSet)
    {
    if(_t.hr < 10) 
    {
      this->lcd.print("0");
    }
    this->lcd.print(_t.hr);
  }
  this->lcd.setCursor(8, 1);
  this->lcd.print(":");
  if (this->minuteSet)
  {
    if(_t.min < 10) 
    {
      this->lcd.print("0");
    }
    this->lcd.print(_t.min);
  }
  
  delay(70);
}

void Interface::weatherPage()
{
  this->getHumidity();
  this->getTemperature();
  this->lcd.clear();

  String _myTemperature = String (this->temperature, 1);
  this->lcd.setCursor(0,0);
  this->lcd.print("T: ");
  this->lcd.print(_myTemperature);
  this->lcd.print((char)223);
  this->lcd.print("C");
  
  if (this->menuButton.index % 3 == 0)
  {
    this->lcd.setCursor(15, 0);
    this->lcd.print("X");
  }

  String _myHumidity = String (this->humidity, 0);
  this->lcd.setCursor(0, 1);
  this->lcd.print("H: ");
  this->lcd.print(_myHumidity);
  this->lcd.print("%");
  delay(100);
}

void Interface::mainPage()
{
  Time _t = this->klok.time();
  
  if(_t.sec < 10 || _t.sec > 20 && _t.sec < 30 || _t.sec > 40 && _t.sec < 50)
  {this->weatherPage();}
  
  else
  {this->clockPage();}
}

//Checking remote Signal
void Interface::checkSignal()
{  
  while (this->IR.decode(&IRresults))
  {
    this->tempSignal = IRresults.value;   
    Serial.println(IRresults.value);
    
    switch(this->tempSignal)
    {
      case Remote::WEATHERPAGE:
      this->menuButton.index = Page::WEATHER;
      break;

      case Remote::CLOCKPAGE:
      this->menuButton.index = Page::CLOCK;
      break;

      case Remote::HOMEPAGE:
      this->menuButton.index = Page::MAIN;
      break;

      case Remote::POWERBUTTON:
      this->IR.resume();
      this->powerButton.index++;
      this->checkPowerButton();
      break;

      case Remote::OPTIONS:
      this->menu();
      break;

      default:
      break;
    }

    if(this->tempSignal != Remote::POWERBUTTON)
    {
    delay(50);
    this->IR.resume();
    }
  }    
}

//check menu button _index and sets the right page to be run
void Interface::runPage()
{
  this->checkSignal();  
  this->checkButtons();
  
  if(this->menuButton.index % 3 == 0)
  {this->weatherPage();}
  
  else if(this->menuButton.index % 3 == 1)
  {this->clockPage();}

  else
  {this->mainPage();}
}

//Buttons 
void Interface::checkPowerButton() 
{
  if(this->powerButton.index % 2 != 0) 
  {
    bool _bLightOn = false;
    digitalWrite(this->BACKLIGHTPIN, LOW);
    
    while(!_bLightOn)
    {
      this->lcd.clear();

      if(this->IR.decode(&this->IRresults))
      {
        this->tempSignal = this->IRresults.value;

        if(this->tempSignal == Remote::POWERBUTTON)
        {
          this->powerButton.index++;
          digitalWrite(this->BACKLIGHTPIN, HIGH);
          _bLightOn = true;
        }
        
        delay(10);
        this->IR.resume();
      }
      
      if(this->powerButton.isPushed())
      {
        this->powerButton.index++;
        digitalWrite(this->BACKLIGHTPIN, HIGH);
        _bLightOn = true;
      }
    }
  }
}

void Interface::checkButtons()
{
  if(this->menuButton.isPushed()) 
  {this->menuButton.index++;}
  
  if(this->powerButton.isPushed()) 
  {this->powerButton.index++;}

  this->checkPowerButton();
}

void Interface::setTime()
{
  Time _t = klok.time();

  this->yearSet = false;
  this->monthSet = false;
  this->daySet = false;
  this->hourSet = false;
  this->minuteSet = false;
  
  int _index = 0;
  uint16_t _digits[10] = {11, 11, 11, 11, 11, 11, 11, 11, 11, 11}; //each element stands for a digit to be set DD-MM-YY HH-MM.
  
  while(this->timeBeingSet && _index < 10)
  {    
    this->clockPage();

    if (this->IR.decode(&this->IRresults))
    {
      this->tempSignal = this->IRresults.value;   
    
      switch(this->tempSignal)
      {      
       case Remote::N0:
       _digits[_index] = 0;
       _index++;
       break; 

       case Remote::N1:
       _digits[_index] = 1;
       _index++;
       break; 

       case Remote::N2:
       _digits[_index] = 2;
       _index++;
       break; 

       case Remote::N3:
       _digits[_index] = 3;
       _index++;
       break; 

       case Remote::N4:
       _digits[_index] = 4;
       _index++;
       break; 

       case Remote::N5:
       _digits[_index] = 5;
       _index++;
       break; 

       case Remote::N6:
       _digits[_index] = 6;
       _index++;
       break; 

       case Remote::N7:
       _digits[_index] = 7;
       _index++;
       break; 

       case Remote::N8:
       _digits[_index] = 8;
       _index++;
       break; 

       case Remote::N9:
       _digits[_index] = 9;
       _index++;
       break; 
       
       case Remote::POWERBUTTON:
       this->timeBeingSet = false;
       break;

       case Remote::NEXTSEGMENT:
       switch(_index)
       {
        case 0:
        _index = 2;
        this->daySet = true; 
        break;

        case 1:
        _index = 2;
        this->daySet = true; 
        break;

        case 2:
        _index = 4;
        this->monthSet = true; 
        break;

        case 3:
        _index = 4;
        this->monthSet = true; 
        break;

        case 4:
        _index = 6;
        this->yearSet = true; 
        break;

        case 5:
        _index = 6;
        this->yearSet = true; 
        break;

        case 6:
        _index = 8;
        this->hourSet = true; 
        break;

        case 7:
        _index = 8;
        this->hourSet = true; 
        break;

        case 8:
        _index = 10;
        this->minuteSet = true; 
        break;

        case 9:
        _index = 10;
        this->minuteSet = true; 
        break;

        default:
        break;
       }
       
       break;

       case Remote::LASTSEGMENT:
       switch(_index)
       {        
        case 1:
        _index = 0;
        this->daySet = false;    
        this->monthSet = false;
        this->yearSet = false;
        this->hourSet = false;
        this->minuteSet = false;   
        break;

        case 2:
        _index = 0;
        this->daySet = false;  
        this->monthSet = false;
        this->yearSet = false;
        this->hourSet = false;
        this->minuteSet = false;     
        break;

        case 3:
        this->monthSet = false;
        _index = 2;
        break;

        case 4:
        this->monthSet = false;
        _index = 2;  
        break;

        case 5:
        this->yearSet = false;
        _index = 4;
        break;

        case 6:
        this->yearSet = false;
        _index = 4;  
        break;

        case 7:
        this->hourSet = false;
        _index = 6;
        break;

        case 8:
        this->hourSet = false;
        _index = 6;  
        break;

        case 9:
        this->minuteSet = false;
        _index = 8;
        break;

        case 10:
        this->minuteSet = false;
        _index = 8;  
        break;

        default:
        break;
       }
       break;       
       
       default:
       break;
    }
      
    switch(_index)
    {
      case 1:
      this->daySet = true;
      this->monthSet = false;
      this->yearSet = false;
      this->hourSet = false;
      this->minuteSet = false;
      
      _t.date = _digits[0];
      this->klok.time(_t);
      break;

      case 2:
      if(_digits[0] < 10 && _digits[1] < 10)
      {
       _t.date = (_digits[0] * 10) + _digits[1]; // if the digit has been set it will run this, otherwise it will pass. 
      }
      
      this->klok.time(_t);
      break;

      case 3:
      this->monthSet = true;
      this->yearSet = false;
      this->hourSet = false;
      this->minuteSet = false;
      _t.mon = _digits[2];
      this->klok.time(_t);
      break;

      case 4:
      if(_digits[2] < 10 && _digits[3] < 10)
      {
      _t.mon = (_digits[2] * 10) + _digits[3];
      }
      this->klok.time(_t);
      break;

      case 5:
      this->yearSet = true;
      this->hourSet = false;
      this->minuteSet = false;
      _t.yr = _digits[4] + 2000;
      this->klok.time(_t);
      break;

      case 6:
      if(_digits[4] < 10 || _digits[5] < 10)
      {
      _t.yr = 2000 + (_digits[4] * 10) + _digits[5];
      }
      this->klok.time(_t);
      break;

      case 7:
      this->hourSet = true;
      this->minuteSet = false;
      _t.hr = _digits[6];
      this->klok.time(_t);
      break;

      case 8:
      if(_digits[6] < 10 || _digits[7] < 10)
      {
      _t.hr = (_digits[6] * 10) + _digits[7];
      }
      this->klok.time(_t);
      break;

      case 9:
      this->minuteSet = true;
      _t.min = _digits[8];
      this->klok.time(_t);
      break;

      case 10:
      if(_digits[8] < 10 || _digits[9] < 10)
      {
      _t.min = (_digits[8] * 10) + _digits[9];
      }
      this->klok.time(_t);
      this->timeBeingSet = false;
      break;

      default:
      break;
    }
    
    delay(50);
    this->IR.resume();
   }      
  }
  this->yearSet = true;
  this->monthSet = true;
  this->daySet = true;
  this->hourSet = true;
  this->minuteSet = true;
}

void Interface::menu()
{
  int _index = 0;
  int _lastIndex;
  bool _menuSelected = true;
  
  while(_menuSelected)
  {
    switch(_index)
    {
      case 0:
      this->lcd.clear();
      this->lcd.setCursor(5, 0);
      this->lcd.print(">Home<");
      this->lcd.setCursor(4,1);
      this->lcd.print("Set Time"); 
      break;

      case 1:
      this->lcd.clear();
      this->lcd.setCursor(6, 0);
      this->lcd.print("Home");
      this->lcd.setCursor(3,1);
      this->lcd.print(">Set Time<"); 
      break;
      
      case 2:
      this->lcd.clear();
      this->lcd.setCursor(4, 0);
      this->lcd.print(">Alarm<");
      this->lcd.setCursor(5,1);
      this->lcd.print("Timer"); 
      break;

      case 3:
      this->lcd.clear();
      this->lcd.setCursor(5, 0);
      this->lcd.print("Alarm");
      this->lcd.setCursor(4,1);
      this->lcd.print(">Timer<"); 
      break;

      default:
      break;
    }

   delay(100);
   _lastIndex = _index; 
   if (this->IR.decode(&IRresults))
   {
    this->tempSignal = IRresults.value;  
    
    switch(this->tempSignal)
    {
      case Remote::N5:
      switch(_index)
      {
        case 0:
        _menuSelected = false;
        break;

        case 1:
        IR.resume();         
        this->timeBeingSet = true;
        this->setTime();
        break;    

        case 3:
        IR.resume();
        delay(50);
        this->setUserTimer();
        break;

        default:        
        break;
      }      
      break;

      case Remote::N8:
      _index++;
      break;

      case Remote::N2:
      _index--;
      break;

      default:
      break;
    }

    if(_index < 0) 
    {
      _index = 0;
    }
    
    else if(_index > 3) 
    {
      _index = 3;
    }
    
    if(this->tempSignal == Remote::N5 && _index != 1 && _index != 3 || this->tempSignal != Remote::N5 )
    {
      this->IR.resume();
      delay(50);
    }    
   }    
  }
 }

 void Interface::timerPage(int _hours, int _minutes, int _seconds, bool _secondsSet)
 {
   this->lcd.clear();
   this->lcd.setCursor(1,0);
   if(!_secondsSet)
   {this->lcd.print("Set Timer: ");}
   else {this->lcd.print("Countdown Timer: ");}
   this->lcd.setCursor(4, 1);
   if(_hours < 10) {this->lcd.print("0");}
   this->lcd.print(_hours);
   this->lcd.setCursor(6, 1);
   this->lcd.print(":");
   if(_minutes < 10) {this->lcd.print("0");}
   this->lcd.print(_minutes);
   this->lcd.setCursor(9, 1);
   this->lcd.print(":");
   if(_seconds < 10) {this->lcd.print("0");}
   this->lcd.print(_seconds);  
   
   delay(80);
 }
 
 void Interface::setUserTimer()
 {
  bool _hoursSet = false;
  bool _minutesSet = false;
  bool _secondsSet = false;
  
  unsigned int _index = 0;

  unsigned int _digits[6] = {0, 0, 0, 0, 0, 0};
  
  int _hours = 0;
  int _minutes = 0;
  int _seconds = 0;
    
  bool _timerSet = false;
  bool _showCountdown = false;
  while(!_timerSet)
  {
    this->timerPage(_hours, _minutes, _seconds, _secondsSet);
    
    //user input
    if(this->IR.decode(&this->IRresults))
    {
      this->tempSignal = this->IRresults.value;
      
      switch(this->tempSignal)
      {
        case Remote::N0:
        _digits[_index] = 0;
        _index++;
        break;

        case Remote::N1:
        _digits[_index] = 1;
        _index++;
        break;

        case Remote::N2:
        _digits[_index] = 2;
        _index++;
        break;

        case Remote::N3:
        _digits[_index] = 3;
        _index++;
        break;

        case Remote::N4:
        _digits[_index] = 4;
        _index++;
        break;

        case Remote::N5:
        _digits[_index] = 5;
        _index++;
        break;

        case Remote::N6:
        _digits[_index] = 6;
        _index++;
        break;

        case Remote::N7:
        _digits[_index] = 7;
        _index++;
        break;

        case Remote::N8:
        _digits[_index] = 8;
        _index++;
        break;

        case Remote::N9:
        _digits[_index] = 9;
        _index++;
        break;  

        case Remote::POWERBUTTON:
        _timerSet = true;
        break; 

        default:
        break;
      }

      switch(_index)
      {
        case 1:
        _hours = _digits[0];
        break;
        
        case 2:
        _hours = (_digits[0] * 10) + _digits[1];
        if(_hours > 23) {_hours = 23;}
        break;

        case 3:
        _minutes = _digits[2];
        break;

        case 4:
        _minutes = (_digits[2] * 10) + _digits[3];
        if(_minutes > 59) {_minutes = 59;}
        break;   

        case 5:
        _seconds = _digits[4];
        break;
                
        case 6:
        _seconds = (_digits[4] * 10) + _digits[5];
        if(_seconds > 59) {_seconds = 59;}

        _secondsSet = true;
        _timerSet = true;
        _showCountdown = true;
        _index = 0;        
        break;

        default: 
        break;
      }
      
      
      delay(50);
      this->IR.resume();
    }
  }
      while(_showCountdown)
      {
        this->timerPage(_hours, _minutes, _seconds, _secondsSet);
        if(this->userTimer.ifTimePassed(1000))
        {
          _seconds--;
          if(_seconds < 0)
          {
            _seconds = 59;
            _minutes--;

            if(_minutes < 0)
            {
              _minutes = 59;
              _hours--;

              if(_hours < 0)
              {
                _showCountdown = false;
              }
            }
          }
        }

        if(this->IR.decode(&this->IRresults))
        {
          this->tempSignal = this->IRresults.value;

          switch(this->tempSignal)
          {
            case Remote::POWERBUTTON:
            _showCountdown = false;
            break;

            default:
            break;
          }

          this->IR.resume();
          delay(50);
        }        
      }
    }
  }
