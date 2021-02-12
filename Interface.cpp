#include "Interface.h"
#include "Remote.h"

namespace LDKlok
{
//startup functions (only run once)
void Interface::lcdStartup()
{
  this->IR.enableIRIn();  // Enables the Infared
  pinMode(this->bLight, OUTPUT);
  this->lcd.begin(16, 2);
  digitalWrite(this->bLight, HIGH);
  this->lcd.setCursor(0, 0);
  this->lcd.print("Verbinding LCD");
  delay(1000);  
}

void Interface::bmeStartup()
{
  bool status;

  status = this->bme.begin(0x76);
  if(!status)
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
  Time t = this->klok.time();
  this->lcd.clear();
  
  if(this->menuButton.index % 3 == 1)
  {
    this->lcd.setCursor(0, 0); 
    this->lcd.print("X");
  }
  this->lcd.setCursor(3, 0);
  if (daySet)
  {
    if(t.date < 10) {this->lcd.print("0");}
    this->lcd.print(t.date);
  }
  this->lcd.setCursor(5, 0);
  this->lcd.print("-");
  if (monthSet)
  {
    if(t.mon < 10) {this->lcd.print("0");}
    this->lcd.print(t.mon);
  }
  this->lcd.setCursor(8, 0);
  this->lcd.print("-");
  if (yearSet)
  {
    this->lcd.print(t.yr);
  }
  else 
  {
    this->lcd.print("20");
  }
  this->lcd.setCursor(6, 1);
  if (hourSet)
    {
    if(t.hr < 10) 
    {
      this->lcd.print("0");
    }
    this->lcd.print(t.hr);
  }
  this->lcd.setCursor(8, 1);
  this->lcd.print(":");
  if (minuteSet)
  {
    if(t.min < 10) 
    {
      this->lcd.print("0");
    }
    this->lcd.print(t.min);
  }
  //lcd.print(":");
  //if(t.sec < 10) {lcd.print("0");}
  //lcd.print(t.sec);                        //can be used to display seconds if wanted
  delay(100);
}

void Interface::weatherPage()
{
  this->getHumidity();
  this->getTemperature();
  this->lcd.clear();

  String myTemperature = String (temperature, 1);
  this->lcd.setCursor(0,0);
  this->lcd.print("T: ");
  this->lcd.print(myTemperature);
  this->lcd.print((char)223);
  this->lcd.print("C");
  
  if (menuButton.index % 3 == 0)
  {
    this->lcd.setCursor(15, 0);
    this->lcd.print("X");
  }

  String myHumidity = String (humidity, 0);
  this->lcd.setCursor(0, 1);
  this->lcd.print("H: ");
  this->lcd.print(myHumidity);
  this->lcd.print("%");
  delay(100);
}

void Interface::mainPage()
{
  Time t = this->klok.time();
  if(t.sec < 10 || t.sec > 20 && t.sec < 30 || t.sec > 40 && t.sec < 50)
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
      case Remote::rWeatherPage:
      this->menuButton.index = Page::WEATHER;
      break;

      case Remote::rClockPage:
      this->menuButton.index = Page::CLOCK;
      break;

      case Remote::rHomePage:
      this->menuButton.index = Page::MAIN;
      break;

      case Remote::rPowerButton:
      this->powerButton.index++;
      break;

      case Remote::rSetTime:
      this->timeBeingSet = true;
      this->setTime();
      break;

      default:
      break;
    }
    
    delay(50);
    this->IR.resume();
  }    
}

//Buttons 
//check menu button index and sets the right page to be run
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
  if(this->powerButton.index % 2 == 0) 
  {digitalWrite(this->bLight, HIGH);} 
  
    else{digitalWrite(this->bLight, LOW);
     
    this->lcd.clear();
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
  Time t = klok.time();
  
  int year, month, day, hour, minute = -1;

  this->yearSet = false;
  this->monthSet = false;
  this->daySet = false;
  this->hourSet = false;
  this->minuteSet = false;
  
  int index = 0;
  uint16_t digits[10];
    
  while(this->timeBeingSet && index < 10)
  {    
    this->clockPage();

    if (this->IR.decode(&IRresults))
    {
      this->tempSignal = IRresults.value;   
    
      switch(this->tempSignal)
      {      
       case Remote::rN0:
       digits[index] = 0;
       index++;
       break; 

       case Remote::rN1:
       digits[index] = 1;
       index++;
       break; 

       case Remote::rN2:
       digits[index] = 2;
       index++;
       break; 

       case Remote::rN3:
       digits[index] = 3;
       index++;
       break; 

       case Remote::rN4:
       digits[index] = 4;
       index++;
       break; 

       case Remote::rN5:
       digits[index] = 5;
       index++;
       break; 

       case Remote::rN6:
       digits[index] = 6;
       index++;
       break; 

       case Remote::rN7:
       digits[index] = 7;
       index++;
       break; 

       case Remote::rN8:
       digits[index] = 8;
       index++;
       break; 

       case Remote::rN9:
       digits[index] = 9;
       index++;
       break; 
       
       case Remote::rPowerButton:
       this->timeBeingSet = false;
       break;
       
       default:
       break;
    }
      
    switch(index)
    {
      case 1:
      this->daySet = true;
      this->monthSet = false;
      this->yearSet = false;
      this->hourSet = false;
      this->minuteSet = false;
      
      t.date = digits[0];
      this->klok.time(t);
      break;

      case 2:
      t.date = (digits[0] * 10) + digits[1];
      this->klok.time(t);
      break;

      case 3:
      this->monthSet = true;
      this->yearSet = false;
      this->hourSet = false;
      this->minuteSet = false;
      t.mon = digits[2];
      this->klok.time(t);
      break;

      case 4:
      t.mon = (digits[2] * 10) + digits[3];
      this->klok.time(t);
      break;

      case 5:
      this->yearSet = true;
      this->hourSet = false;
      this->minuteSet = false;
      t.yr = digits[4] + 2000;
      this->klok.time(t);
      break;

      case 6:
      t.yr = 2000 + (digits[4] * 10) + digits[5];
      this->klok.time(t);
      break;

      case 7:
      this->hourSet = true;
      this->minuteSet = false;
      t.hr = digits[6];
      this->klok.time(t);
      break;

      case 8:
      t.hr = (digits[6] * 10) + digits[7];
      this->klok.time(t);
      break;

      case 9:
      this->minuteSet = true;
      t.min = digits[8];
      this->klok.time(t);
      break;

      case 10:
      t.min = (digits[8] * 10) + digits[9];
      this->klok.time(t);
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
}
