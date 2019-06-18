// fading transitions sketch for 6-tube IN-17 board with default connections.
// based on 6-tube sketch by Emblazed
// 
// 02/27/2013 - modded for six bulb board, updated flicker fix by Brad L
//
// 2019/6/02 - Adapted for Blynk and WiFi connectivity by Gage Leighton however this requires WiFi to be available so not ideal

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
int ledPin_a_1 = 13;
int ledPin_a_2 = 12;
int ledPin_a_3 = 11;
int ledPin_a_4 = 10;

////////////////////// Blynk Section //////////////////////////////////
//
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <WidgetRTC.h>

// You should get Auth Tokein in the Blynk App on your phone
// Go to the Progect Settings (nut icon)
char auth[] = "auth token";

// Your WiFi credentials.
// Set password to "" for open networks
char ssid[] = "wifi name";
char pass[] = "password";

// Hardware serial on Mega, Lenoardo, Micro...
#define EspSerial Serial1

// or Software Serial on Uno, Nano...
//#include <SoftwareSerial.h>
//SoftwareSerial EspSerial(51, 53); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

BlynkTimer timer1;
BlynkTimer timer2;

WidgetRTC rtc;

// For future clock display settings (12hr vs 24hr, blink on hour, etc.)
BLYNK_WRITE(V1){

}

/////////////////////// RTC Section ///////////////////////////////////////////////////////
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc1;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Global Variables
byte t_sec, t_min, t_hour, t_weekday, t_day, t_month, t_year;
byte zero;

//void requestTime() {
//	rtc.sendInterval("rtc","sync");
// setDS3231();
//}

//BLYNK_WRITE(InternalPinRTC){
//	long t = param.asLong();
//	Serial.print("Unix time: ");
//	Serial.print(t);
//	Serial.println();
//}

BLYNK_CONNECTED() {
  // Synchorinze time on connection
  rtc.begin();
  //setDS3231();
}

// Gets the date and time from the ds3231
void getDateDs3231() {
  DateTime now = rtc1.now();
  t_sec     = now.second();
  t_min     = now.minute();
  t_hour    = now.hour();
  t_weekday = daysOfTheWeek[now.dayOfTheWeek()];
  t_day     = now.day();
  t_month   = now.month();
  t_year    = now.year();
}

void setup() 
{
	// Debug console
	Serial.begin(9600);

  // Begin the RTC chip
  if (! rtc1.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
//  if (rtc1.lostPower()) {
//    Serial.println("RTC lost power, lets set the time!");
//    // following line sets the RTC to the date & time this sketch was compiled
//    rtc1.adjust(DateTime(F(__DATE__), F(__TIME__)));
//    // This line sets the RTC with an explicit date & time, for example to set
//    // January 21, 2014 at 3am you would call:
//    // rtc1.adjust(DateTime(2014, 1, 21, 3, 0, 0));
//  }
  
	// Set ESP8266 baud rate
	EspSerial.begin(ESP8266_BAUD);
	delay(10);

	Blynk.begin(auth, wifi, ssid, pass);
	// You can also specify the server
	//Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 80);
	//Blynk.begin(auth, wifi, ssid, pass, IPAddress(192.168.1.100), 8080);
	
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

  setSyncInterval(60); // Sync interval in seconds

	timer1.setInterval(60000L, setDS3231); // Set RTC from Blynk server (if connected) ever minute
	timer2.setInterval(900L, DisplayFadeNumberString); // Update the Nixie display every 900wseaqdms
}

void SetSN74141Chips( int num2, int num1 )
{
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

void setDS3231(){
	if (Blynk.connected()){
		rtc1.adjust(DateTime(year(),month(),day(),hour(),minute(),second()));
	}
}

float fadeMax = 5.0f;
float fadeStep = 1.0f;
int NumberArray[6]={0,0,0,0,0,0};
int currNumberArray[6]={0,0,0,0,0,0};
float NumberArrayFadeInValue[6]={1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
float NumberArrayFadeOutValue[6]={8.0f,8.0f,8.0f,8.0f,8.0f,8.0f};

void DisplayFadeNumberString()
{
	getDateDs3231();
  Serial.print(t_hour);
  Serial.print(" : ");
  Serial.print(t_min);
  Serial.print(" : ");
  Serial.println(t_sec);
	// Fill in the array used to display the tubes.
	NumberArray[5] = t_hour/10 % 10;
	NumberArray[4] = t_hour % 10;
	NumberArray[3] = t_min/10 % 10;
	NumberArray[2] = t_min % 10;
	NumberArray[1] = t_sec/10 % 10;
	NumberArray[0] = t_sec % 10;

	// Anode channel 1 - numerals 0,3
	SetSN74141Chips(currNumberArray[0],currNumberArray[3]);
	digitalWrite(ledPin_a_2, HIGH);
	delay(NumberArrayFadeOutValue[0]);
	SetSN74141Chips(NumberArray[0],NumberArray[3]);
	delay(NumberArrayFadeInValue[0]);
	digitalWrite(ledPin_a_2, LOW);

	// Anode channel 2 - numerals 1,4
	SetSN74141Chips(currNumberArray[1],currNumberArray[4]);
	digitalWrite(ledPin_a_3, HIGH);
	delay(NumberArrayFadeOutValue[1]);
	SetSN74141Chips(NumberArray[1],NumberArray[4]);
	delay(NumberArrayFadeInValue[1]);
	digitalWrite(ledPin_a_3, LOW);

	// Anode channel 3 - numerals 2,5
	SetSN74141Chips(currNumberArray[2],currNumberArray[5]);
	digitalWrite(ledPin_a_4, HIGH);
	delay(NumberArrayFadeOutValue[2]);
	SetSN74141Chips(NumberArray[2],NumberArray[5]);
	delay(NumberArrayFadeInValue[2]);
	digitalWrite(ledPin_a_4, LOW);

	// Loop thru and update all the arrays, and fades.
	for( int i = 0 ; i < 6 ; i ++ )
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

////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void loop()     
{
	Blynk.run();
	timer1.run();
	timer2.run();
}
