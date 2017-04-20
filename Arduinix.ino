// Arduinix - Nixie tube display clock
// V1.2 
// Revision by gageoleighton
// fading transitions sketch for 8-tube board with default connections.
// based on 6-tube sketch by Emblazed
// 4-tube-itized by Dave B. 16 June 2011
// this shows minutes and seconds only

#include "Wire.h"
// Include RTC from adafuit
#include <RTClib.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// SN74141 : Truth Table
//D C B A #
//L,L,L,L 0
//L,L,L,H 1
//L,L,H,L 2
//L,L,H,H 3
//L,H,L,L 4
//L,H,L,H 5
//L,H,H,L 6
//L,H,H,H 7
//H,L,L,L 8
//H,L,L,H 9

int ledPin_0_a = 2;                
int ledPin_0_b = 3;
int ledPin_0_c = 4;
int ledPin_0_d = 5;

int ledPin_1_a = 6;                
int ledPin_1_b = 7;
int ledPin_1_c = 8;
int ledPin_1_d = 9;

int ledPin_a_1 = 10;
int ledPin_a_2 = 11;
int ledPin_a_3 = 12;
int ledPin_a_4 = 13;


void setup() 
{
  
  pinMode(ledPin_0_a, OUTPUT);      
  pinMode(ledPin_0_b, OUTPUT);      
  pinMode(ledPin_0_c, OUTPUT);      
  pinMode(ledPin_0_d, OUTPUT);    
  
  pinMode(ledPin_1_a, OUTPUT);      
  pinMode(ledPin_1_b, OUTPUT);      
  pinMode(ledPin_1_c, OUTPUT);      
  pinMode(ledPin_1_d, OUTPUT);      
  
  pinMode(ledPin_a_1, OUTPUT);      
  pinMode(ledPin_a_2, OUTPUT);      
  pinMode(ledPin_a_3, OUTPUT);     
  
 // Initialize I2C
  Wire.begin();

  //////////////////////////////////////////////////////////////
  //  set the initial time here:                              //
  //  DS3231 seconds, minutes, hours, day, date, month, year  //
  //////////////////////////////////////////////////////////////
  // setDS3231time(30,42,21,4,26,11,14);

 
  // If arduino looses power and battery dies reset time to when script was uploaded
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}

void SetSN74141Chips( int num2, int num1 ){
  int a,b,c,d;
  
  // set defaults.
  a=0;b=0;c=0;d=0; // will display a zero.
  
  // Load the a,b,c,d.. to send to the SN74141 IC (1)
  switch( num1 )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
    default: a=1;b=1;c=1;d=1;
    break;
  }  
  
  // Write to output pins.
  digitalWrite(ledPin_0_d, d);
  digitalWrite(ledPin_0_c, c);
  digitalWrite(ledPin_0_b, b);
  digitalWrite(ledPin_0_a, a);

  // Load the a,b,c,d.. to send to the SN74141 IC (2)
  switch( num2 )
  {
    case 0: a=0;b=0;c=0;d=0;break;
    case 1: a=1;b=0;c=0;d=0;break;
    case 2: a=0;b=1;c=0;d=0;break;
    case 3: a=1;b=1;c=0;d=0;break;
    case 4: a=0;b=0;c=1;d=0;break;
    case 5: a=1;b=0;c=1;d=0;break;
    case 6: a=0;b=1;c=1;d=0;break;
    case 7: a=1;b=1;c=1;d=0;break;
    case 8: a=0;b=0;c=0;d=1;break;
    case 9: a=1;b=0;c=0;d=1;break;
    default: a=1;b=1;c=1;d=1;
    break;
  }
  
  // Write to output pins
  digitalWrite(ledPin_1_d, d);
  digitalWrite(ledPin_1_c, c);
  digitalWrite(ledPin_1_b, b);
  digitalWrite(ledPin_1_a, a);
}

/////////////////////////////////////////////////////////////////////////
//
// DisplayNumberString
// Use: passing an array that is 4 elements long will display numbers
//      on a 4 nixie bulb setup.
//
/////////////////////////////////////////////////////////////////////////
float fadeIn = 0.0f;
float fadeOut = 8.0f;
float fadeMax = 8.0f;
float fadeStep = 1.0f;
int NumberArray[4]={0,0,0,0};
int currNumberArray[4]={0,0,0,0};
float NumberArrayFadeInValue[4]={0.0f,0.0f,0.0f,0.0f};
float NumberArrayFadeOutValue[4]={8.0f,8.0f,8.0f,8.0f};

void DisplayFadeNumberString()
{
  // Nixie setup..
  
  // NOTE: If any of the bulbs need to blend then it will
  // be in time with the seconds bulbs. because any change only happens
  // on a one second interval. 
  
  // Anode 1 (0,3)
  SetSN74141Chips(currNumberArray[0],currNumberArray[3]);   
  digitalWrite(ledPin_a_1, HIGH);   
  delay(NumberArrayFadeOutValue[0]);
  SetSN74141Chips(NumberArray[0],NumberArray[3]);   
  delay(NumberArrayFadeInValue[0]);
  digitalWrite(ledPin_a_1, LOW);
  
  // Anode 2 (1,4)
  SetSN74141Chips(currNumberArray[1],currNumberArray[4]);   
  digitalWrite(ledPin_a_2, HIGH);   
  delay(NumberArrayFadeOutValue[1]);
  SetSN74141Chips(NumberArray[1],NumberArray[4]);   
  delay(NumberArrayFadeInValue[1]);
  digitalWrite(ledPin_a_2, LOW);

    // Anode 3 (2,5)
  SetSN74141Chips(currNumberArray[2],currNumberArray[5]);   
  digitalWrite(ledPin_a_3, HIGH);   
  delay(NumberArrayFadeOutValue[3]);
  SetSN74141Chips(NumberArray[2],NumberArray[5]);   
  delay(NumberArrayFadeInValue[3]);
  digitalWrite(ledPin_a_3, LOW);
  
  // Loop thru and update all the arrays, and fades.
  for( int i = 0 ; i < 4 ; i ++ )
  {
    if( NumberArray[i] != currNumberArray[i] )
    {
      NumberArrayFadeInValue[i] += fadeStep;
      NumberArrayFadeOutValue[i] -= fadeStep;
  
      if( NumberArrayFadeInValue[i] >= fadeMax )
      {
        NumberArrayFadeInValue[i] = 0.0f;
        NumberArrayFadeOutValue[i] = fadeMax;
        currNumberArray[i] = NumberArray[i];
      }
    }
  }  
}

int readButtons(int pin,int pin2)
// returns the button number pressed, or zero for none pressed 
// int pin is the analog pin number to read 
{
  int b,c,d = 0;
  c=digitalRead(pin); // get the digital value  1
  d=digitalRead(pin2); // get digital value 2

  if (c==HIGH & d==LOW){
    b=1; // button 1 pressed
  }
      else if (c==LOW && d==HIGH){
          b=2; // button 2 pressed
        }           
        else if (d==LOW & c==LOW){
            b=0; // no buttons pressed
          }
          else if (c==HIGH & d==HIGH){
            b=4; //both buttons pressed
          }
return b;
}

// Defines
long MINS = 60;         // 60 Seconds in a Min.
long HOURS = 60 * MINS; // 60 Mins in an hour.
long DAYS = 12 * HOURS; // 24 Hours in a day. > Note: change the 24 to a 12 for non millitary time.

long runTime = 0;       // Time from when we started.

// default time sets
long clockHourSet;
long clockMinSet;


void setrtc(int sethour, int setmin){
  DateTime now = rtc.now();
  rtc.adjust(DateTime(now.year(),now.month(),now.day(),sethour,setmin,now.second()));
}

  // Buttons
  int Button = 0;
  int old_Button = 0;

void loop()     
{
 
  DateTime now = rtc.now(); 
 
  Button=old_Button;

  // Convert time in days,hours,mins,seconds
  long days  = now.day();
  long hours = now.hour(); 
  long minutes  = now.minute(); 
  long seconds  = now.second(); 

  // Get the high and low order values for hours,min,seconds. 
  int lowerHours = hours % 10;
  int upperHours = hours - lowerHours;
  int lowerMins = minutes % 10;
  int upperMins = minutes - lowerMins;
  int lowerSeconds = seconds % 10;
  int upperSeconds = seconds - lowerSeconds;
  if( upperSeconds >= 10 )   upperSeconds = upperSeconds / 10;
  if( upperMins >= 10 )      upperMins = upperMins / 10;
  if( upperHours >= 10 )     upperHours = upperHours / 10;
  
  if( upperHours == 0 && lowerHours == 0 )
  {
    upperHours = 1;
    lowerHours = 2;
  }
  // Fill in the Number array used to display on the tubes.
  NumberArray[3] = upperHours;
  NumberArray[2] = lowerHours;
  NumberArray[1] = upperMins;
  NumberArray[0] = lowerMins;

  // Display.
  DisplayFadeNumberString();

// Get state of buttons
  Button = readButtons(A0,A1);
  
  if (Button != old_Button){
    clockMinSet = now.minute();
    clockHourSet = now.hour();
    if(Button == 0){}
    else if(Button == 1){
      if(now.minute() != 59){
      clockMinSet++;}
      else {
        clockMinSet = 0;
      }
      setrtc(clockHourSet,clockMinSet);
      }
      else if(Button == 2){
        if(now.hour() != 23){
          clockHourSet++;}
          else {
            clockHourSet = 0;
          }
      setrtc(clockHourSet,clockMinSet);
      }
      else if(Button == 4){
        if(now.minute() != 0){
        clockMinSet--;}
        else {
          clockMinSet = 59;
        }
        setrtc(clockHourSet,clockMinSet);
      }
    delay(100);
  }
}
