/////////////////////////////
/////// Arduinix RTC ////////
/////////////////////////////
//
// By Gage Leighton
//    2020-05-04 - May the 4th be with you
//
// Need to program in case of RTC failure
//  use internal function
//


////// RTC Section //////
// Date and time functions using a DS3231 RTC connected via I2C
#include "RTClib.h" // https://github.com/adafruit/RTClib
RTC_DS3231 rtc;


////// Global Variables //////
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
byte t_sec, t_min, t_hour, t_weekday, t_day, t_month, t_year;
byte zero;

// Fade in/out times .. Values set for IN-17 Nixie Bulbs
float fadeMax = 6.0f;
float fadeStep = fadeMax / 8;
int NumberArray[8]={0,0,0,0,0,0,0,0};
int currNumberArray[8]={0,0,0,0,0,0,0,0};
float NumberArrayFadeInValue[8]={0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
float NumberArrayFadeOutValue[8]={5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,5.0f};

// Button states
bool HourButtonPresseda = false;
bool HourButtonPressedb = false;
bool MinButtonPresseda = false;
bool MinButtonPressedb = false;
bool SecButtonPresseda = false;
bool SecButtonPressedb = false;

// Clock uptime and cycle
unsigned long runtime = 0;
int deltat = 100;
bool time_changed = false;

////// Pins //////
// SN74141 (1)
int ledPin_0_a = 2;
int ledPin_0_b = 3;
int ledPin_0_c = 4;
int ledPin_0_d = 5;

// SN74141 (2)
int ledPin_1_a = 6;
int ledPin_1_b = 7;
int ledPin_1_c = 8;
int ledPin_1_d = 9;

// anode pins
int ledPin_a_1 = 10;
int ledPin_a_2 = 11;
int ledPin_a_3 = 12;
int ledPin_a_4 = 13;

////// Functions //////

// Gets the date and time from the ds3231
void getDateDs3231() {
  DateTime dt = rtc.now();
  t_sec = dt.second();
  t_min = dt.minute();
  t_hour = dt.hour();
  t_day = dt.day();
  t_month = dt.month();
  t_year = dt.year();
}

void getTime() {
  getDateDs3231();
  Serial.print(t_hour);
  Serial.print(" : ");
  Serial.print(t_min);
  Serial.print(" : ");
  Serial.println(t_sec);
  // Fill in the array used to display the tubes.
  NumberArray[7] = t_hour / 10 % 10;
  NumberArray[6] = t_hour % 10;
  NumberArray[5] = t_min / 10 % 10;
  NumberArray[3] = t_min % 10;
  NumberArray[2] = t_sec / 10 % 10;
  NumberArray[1] = t_sec % 10;
}

void setup() {
  Serial.begin(9600);

// Initialize the RTC chip
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
// Use to initially set RTC then comment out and flash again
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

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
  pinMode(ledPin_a_4, OUTPUT);


// Input and output pin section
  pinMode(A0, INPUT); // SET HOURS up - A0
  digitalWrite(A0, HIGH); // set pin as a pull up resistor.

  pinMode(A1, INPUT); // SET HOURS down - A1
  digitalWrite(A1, HIGH); // set pin as a pull up resistor.

  pinMode(A2, INPUT); // SET MINUTES up - A2
  digitalWrite(A2, HIGH); // set pin as a pull up resistor.

  pinMode(A3, INPUT); // SET MINUTES down - A3
  digitalWrite(A3, HIGH); // set pin as a pull up resistor.

  pinMode(A4, INPUT); // SET SECONDS up - A4
  digitalWrite(A4, HIGH); // set pin as a pull up resistor.

  pinMode(A5, INPUT); // SET SECONDS down - A5
  digitalWrite(A5, HIGH); // set pin as a pull up resistor.

  Serial.println("End startup");

}

void SetSN74141Chips(int num2, int num1) {
  int a, b, c, d;

  // set defaults.
  a = 0;
  b = 0;
  c = 0;
  d = 0;

  // Load the a,b,c,d.. to send to the SN74141 IC (1)
  switch (num1) {
  case 0: a=0; b=0; c=0; d=0; break;
  case 1: a=1; b=0; c=0; d=0; break;
  case 2: a=0; b=1; c=0; d=0; break;
  case 3: a=1; b=1; c=0; d=0; break;
  case 4: a=0; b=0; c=1; d=0; break;
  case 5: a=1; b=0; c=1; d=0; break;
  case 6: a=0; b=1; c=1; d=0; break;
  case 7: a=1; b=1; c=1; d=0; break;
  case 8: a=0; b=0; c=0; d=1; break;
  case 9: a=1; b=0; c=0; d=1; break;
  default: a=1; b=1; c=1; d=1; break;
  }


  // Write to output pins.
  digitalWrite(ledPin_0_d, d);
  digitalWrite(ledPin_0_c, c);
  digitalWrite(ledPin_0_b, b);
  digitalWrite(ledPin_0_a, a);

  // Set a,b,c,d.. to send to the SN74141 IC (2)
  switch (num2) {
  case 0: a=0; b=0; c=0; d=0; break;
  case 1: a=1; b=0; c=0; d=0; break;
  case 2: a=0; b=1; c=0; d=0; break;
  case 3: a=1; b=1; c=0; d=0; break;
  case 4: a=0; b=0; c=1; d=0; break;
  case 5: a=1; b=0; c=1; d=0; break;
  case 6: a=0; b=1; c=1; d=0; break;
  case 7: a=1; b=1; c=1; d=0; break;
  case 8: a=0; b=0; c=0; d=1; break;
  case 9: a=1; b=0; c=0; d=1; break;
  default: a=1; b=1; c=1; d=1; break;
  }

  // Write to output pins
  digitalWrite(ledPin_1_d, d);
  digitalWrite(ledPin_1_c, c);
  digitalWrite(ledPin_1_b, b);
  digitalWrite(ledPin_1_a, a);
}

void DisplayFadeNumberString() {

  // Anode channel 2 - numerals 1,5
  SetSN74141Chips(currNumberArray[1], currNumberArray[5]);
  digitalWrite(ledPin_a_2, HIGH);
  delay(NumberArrayFadeOutValue[1]);
  SetSN74141Chips(NumberArray[1], NumberArray[5]);
  delay(NumberArrayFadeInValue[1]);
  digitalWrite(ledPin_a_2, LOW);

  // Anode channel 3 - numerals 2,6
  SetSN74141Chips(currNumberArray[2], currNumberArray[6]);
  digitalWrite(ledPin_a_3, HIGH);
  delay(NumberArrayFadeOutValue[2]);
  SetSN74141Chips(NumberArray[2], NumberArray[6]);
  delay(NumberArrayFadeInValue[2]);
  digitalWrite(ledPin_a_3, LOW);

  // Anode channel 4 - numerals 3,7
  SetSN74141Chips(currNumberArray[3], currNumberArray[7]);
  digitalWrite(ledPin_a_4, HIGH);
  delay(NumberArrayFadeOutValue[3]);
  SetSN74141Chips(NumberArray[3], NumberArray[7]);
  delay(NumberArrayFadeInValue[3]);
  digitalWrite(ledPin_a_4, LOW);

  // Loop thru and update all the arrays, and fades.
  for (int i = 0; i < 8; i++) //equal to & of digits
  {
    if (NumberArray[i] != currNumberArray[i]) {
      NumberArrayFadeInValue[i] += fadeStep;
      NumberArrayFadeOutValue[i] -= fadeStep;

      if (NumberArrayFadeInValue[i] >= fadeMax) {
        NumberArrayFadeInValue[i] = fadeStep;
        NumberArrayFadeOutValue[i] = fadeMax; //affects the refresh cycle
        currNumberArray[i] = NumberArray[i];
      }
    }
  }
}


void loop() {
  int hourInput = digitalRead(A0);
  int hourInputD = digitalRead(A1);
  int minInput = digitalRead(A2);
  int minInputD = digitalRead(A3);
  int secInput = digitalRead(A4);
  int secInputD = digitalRead(A5);


  if (hourInput == 0) HourButtonPresseda = true;
  if (minInput == 0) MinButtonPresseda = true;
  if (secInput == 0) SecButtonPresseda = true;

  if (HourButtonPresseda == true && hourInput == 1) {
    if (t_hour == 23) t_hour = 0;
    else t_hour++;
    HourButtonPresseda = false;
    time_changed = true;
  }

  if (MinButtonPresseda == true && minInput == 1) {
    if (t_min == 59) {
      if (t_hour == 23) t_hour = 0;
      else t_hour++;
      t_min = 0;
    } else t_min++;
    MinButtonPresseda = false;
    time_changed = true;
  }

  if (SecButtonPresseda == true && secInput == 1) {
    if (t_sec == 59) {
      if (t_min == 59) {
        t_min = 0;
        if (t_hour == 23) t_hour = 0;
        else t_hour++;
      } else t_min++;
      t_sec = 0;
    } else t_sec++;
    SecButtonPresseda = false;
    time_changed = true;
}

  if (hourInputD == 0) HourButtonPressedb = true;
  if (minInputD == 0) MinButtonPressedb = true;
  if (secInputD == 0) SecButtonPressedb = true;

  if (HourButtonPressedb == true && hourInputD == 1) {
    if (t_hour == 0) t_hour = 23;
    else t_hour--;
    HourButtonPressedb = false;
    time_changed = true;
  }
  
  if (MinButtonPressedb == true && minInputD == 1) {
    if (t_min == 0) {
      if (t_hour == 0) t_hour = 23;
      else t_hour--;
      t_min = 59;
    } else t_min--;
    MinButtonPressedb = false;
    time_changed = true;
  }

  if (SecButtonPressedb == true && secInputD == 1) {
    if (t_sec == 0) {
      if (t_min == 0) {
        t_min = 59;
        if (t_hour == 0) t_hour = 23;
        else t_hour--;
      } else t_min--;
      t_sec = 59;
    } else t_sec--;
    SecButtonPressedb = false;
    time_changed = true;
  }

  if (millis() - runtime > deltat) {
    if (time_changed){
      rtc.adjust(DateTime(t_year, t_month, t_day, t_hour, t_min, t_sec));
//      Serial.println("RTC adjusted");
      time_changed = false;
    }
    getTime();
    runtime = millis();
  }

  DisplayFadeNumberString();
}
