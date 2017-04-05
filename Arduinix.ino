// Arduinix - Nixie tube display clock
// Latest revision by gageoleighton
// fading transitions sketch for 8-tube board with default connections.
// based on 6-tube sketch by Emblazed
// 4-tube-itized by Dave B. 16 June 2011
// this shows minutes and seconds only

#include "Wire.h"
// Include RTC from adafuit
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

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

  // Set analog Reference
  analogReference(EXTERNAL); //Pin jummered to 5V (VCC)
 
  // NOTE:
  // Grounding on virtual pins 14 and 15 (analog pins 0 and 1) will set the Hour and Mins.
  pinMode( 14, INPUT ); // set the vertual pin 14 (pin 0 on the analog inputs ) 
  digitalWrite(14, HIGH); // set pin 14 as a pull up resistor.

  pinMode( 15, INPUT ); // set the vertual pin 15 (pin 1 on the analog inputs ) 
  digitalWrite(15, HIGH); // set pin 15 as a pull up resistor.

  Wire.begin();
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(30,42,21,4,26,11,14);
 
}

void SetSN74141Chips( int num2, int num1 )
{
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


void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

////////////////////////////////////////////////////////////////////////
//
// DisplayNumberString
// Use: passing an array that is 4 elements long will display numbers
//      on a 4 nixie bulb setup.
//
////////////////////////////////////////////////////////////////////////
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
  
  // 1 (0,3)
  SetSN74141Chips(currNumberArray[0],currNumberArray[3]);   
  digitalWrite(ledPin_a_1, HIGH);   
  delay(NumberArrayFadeOutValue[0]);
  SetSN74141Chips(NumberArray[0],NumberArray[3]);   
  delay(NumberArrayFadeInValue[0]);
  digitalWrite(ledPin_a_1, LOW);
  
  // 2 (1,2)
  SetSN74141Chips(currNumberArray[1],currNumberArray[2]);   
  digitalWrite(ledPin_a_2, HIGH);   
  delay(NumberArrayFadeOutValue[1]);
  SetSN74141Chips(NumberArray[1],NumberArray[2]);   
  delay(NumberArrayFadeInValue[1]);
  digitalWrite(ledPin_a_2, LOW);
  
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

// Defines
long MINS = 60;         // 60 Seconds in a Min.
long HOURS = 60 * MINS; // 60 Mins in an hour.
long DAYS = 12 * HOURS; // 24 Hours in a day. > Note: change the 24 to a 12 for non millitary time.

long runTime = 0;       // Time from when we started.

// default time sets. clock will start at 12:59:00
long clockHourSet = 12;
long clockMinSet  = 34;

int HourButtonPressed = false;
int MinButtonPressed = false;

////////////////////////////////////////////////////////////////////////
//
// NEED TO INCORPORATE RTC
////////////////////////////////////////////////////////////////////////
void loop()     
{
  // Get milliseconds.
  runTime = millis();
  
  int hourInput = digitalRead(14);  
  int minInput  = digitalRead(15);

  if( hourInput == 0 )
    HourButtonPressed = true;
  if( minInput == 0 )
    MinButtonPressed = true;
  
  if( HourButtonPressed == true && hourInput == 1 )
  {
    clockHourSet++;
    HourButtonPressed = false;
  }
  
  if( MinButtonPressed == true && minInput == 1 )
  {
    clockMinSet++;
    MinButtonPressed = false;
  }

  // Get time in seconds.
  long time = (runTime) / 1000;
  
  // Set time based on offset..
  // long hbump = 60*60*clockHourSet;
  long hbump = 60*60*clockHourSet;
  long mbump = 60*clockMinSet;
  time += mbump + hbump;

  // Convert time to days,hours,mins,seconds
  long days  = time / DAYS;    time -= days  * DAYS; 
  long hours = time / HOURS;   time -= hours * HOURS; 
  long minutes  = time / MINS;    time -= minutes  * MINS; 
  long seconds  = time; 

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
}
