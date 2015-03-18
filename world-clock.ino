/*

	Dani's New Awesome Clock
	Code rewrite

*/

//RTC Library
#include <Wire.h>		// Wire Library
#include "ds3231.h"		// RTC Library
struct ts t;

// DHT22
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LCD Config
#include "U8glib.h"
U8GLIB_ST7920_128X64_1X u8g(6,5,4,7);

uint8_t secset = 0; 	//Index for second RTC setting
uint8_t minset = 1; 	//Index for minute RTC setting
uint8_t hourset = 2; 	//Index for hour RTC setting
uint8_t wdayset = 3; 	//Index for weekday RTC setting
uint8_t mdayset = 4; 	//Index for date RTC setting
uint8_t monset = 5; 	//Index for month RTC setting
uint8_t yearset = 6;	 //Index for year RTC setting


void setup(){
    Serial.begin(9600);				// Start serial
    Wire.begin();					// Start wire library
    DS3231_init(DS3231_INTCN);		// Start RTC library
    
      //DHT Start
  dht.begin();
  
    //If you want to set the time on the RTC librat, uncomment here:
//	t.mday = 18;
//	t.mon = 3;
//	t.hour = 23;
//	t.min = 3; 
//	t.year = 2015;
//	set_rtc_field(t, hourset);
//	set_rtc_field(t, minset);
//	set_rtc_field(t, yearset);
//      set_rtc_field(t, mdayset);
//      set_rtc_field(t, monset);
}

void loop(){
        //Let' draw
	u8g.firstPage();  
	do { draw(); } 
	while( u8g.nextPage() );
	delay(100);
}


void draw(void){

	/* Here the magic happens */

	DS3231_get(&t); // grab the time from the RTC
	int second	= t.sec;
	int minute	= t.min;
	int hour	= t.hour;
	int day 	= t.mday;
	int month	= t.mon;
	int year	= t.year;

	/* Country fuse difference */

	int BRdiff = 14;
	int USdiff = 15;
	int FRdiff = 10;
	int FIdiff = 9;

	int hourBR;
	int hourUS;
	int hourFI;
	int hourFR;

	// Time Melbourne
	int y = 4; // Move
	char hours[3] 	= {(hour / 10) + 48, (hour % 10) + 48, 0};
	char minutes[3] = {(minute / 10) + 48, (minute % 10) + 48, 0};
	char seconds[3] = {(second / 10) + 48, (second % 10) + 48, 0};
	char years[3] = {((year-2000) / 10) + 48, ((year-2000) % 10) + 48, 0};
	char months[3]	= {(month / 10) + 48, (month % 10) + 48, 0};
	char days[3]	= {(day / 10) + 48, (day % 10) + 48, 0};

	u8g.setFont(u8g_font_freedoomr10r);
	u8g.drawStr( 5+y,  15, hours);
	u8g.drawStr( 18+y, 15,":");
	u8g.drawStr( 24+y, 15, minutes);
	u8g.drawStr( 37+y, 15,":");
	u8g.drawStr( 43+y, 15, seconds);

	// Date Melbourne
	u8g.drawStr( 7,  30, days);
	u8g.drawStr( 22, 30, "/");
	u8g.drawStr( 31, 30, months);
	u8g.drawStr( 46, 30, "/");
	u8g.drawStr( 50, 30, years );
 // Here the magic happens
  u8g.setFont(u8g_font_6x12);

  // Temperature
  float t = dht.readTemperature();
  char tS[5];
  dtostrf(t,5,1,tS);
  int x = 75; 
  u8g.drawStr( 8 + x, 10,tS);
  u8g.drawStr( 0 + x, 10,"T:");
  u8g.drawStr( 38 + x, 10,"\260C");

  // Humidity
  float h = dht.readHumidity();
  char hS[5];
  dtostrf(h,5,1,hS);
  u8g.drawStr( 0+x, 20,"H:");
  u8g.drawStr( 38+x, 20,"%");
  u8g.drawStr( 8+x, 20,hS);

  //Draw Lines
  u8g.drawLine(x-5, 0, x-5, 23);
  u8g.drawLine(x-5, 23, 128, 23);

  // Country Names
  u8g.drawStr( 10, 45,"BR"); // Brazil
  u8g.drawStr( 70, 45,"US"); // USA
  u8g.drawStr( 10, 60,"FI"); // Finland
  u8g.drawStr( 70, 60,"FR"); // France

  // Time in different countries
  // BRAZIL
  if(hour-BRdiff < 0){ hourBR = hour - BRdiff + 24; } else { hourBR = hour - BRdiff;  }
  char hoursBR[3] = {(hourBR / 10) + 48, (hourBR % 10) + 48, 0};
  u8g.drawStr( 25, 45, hoursBR);
  u8g.drawStr( 37, 45,":");
  u8g.drawStr( 42, 45, minutes);

  // USA
  if(hour-USdiff < 0){ hourUS = hour - USdiff + 24; } else { hourUS = hour - USdiff;  }
  char hoursUS[3] = {(hourUS / 10) + 48, (hourUS % 10) + 48, 0};
  u8g.drawStr(  85, 45, hoursUS);
  u8g.drawStr(  97, 45,":");
  u8g.drawStr( 102, 45, minutes);
  
  // France
  if(hour-FRdiff < 0){ hourFR = hour - FRdiff + 24; } else { hourFR = hour - FRdiff;  }
  char hoursFR[3] = {(hourFR / 10) + 48, (hourFR % 10) + 48, 0};
  u8g.drawStr(  85, 60, hoursFR);
  u8g.drawStr(  97, 60,":");
  u8g.drawStr( 102, 60, minutes);
  
  // Finland
  if(hour-FIdiff < 0){ hourFI = hour - FIdiff + 24; } else { hourFI = hour - FIdiff;  }
  char hoursFI[3] = {(hourFI / 10) + 48, (hourFI % 10) + 48, 0};
  u8g.drawStr( 25, 60, hoursFI);
  u8g.drawStr( 37, 60,":");
  u8g.drawStr( 42, 60, minutes);
 
}



//Subroutine to adjust a single date/time field in the RTC
void set_rtc_field(struct ts t,  uint8_t index)
{
    uint8_t century;

    if (t.year > 2000) {
        century = 0x80;
        t.year_s = t.year - 2000;
    } else {
        century = 0;
        t.year_s = t.year - 1900;
    }

    uint8_t TimeDate[7] = { t.sec, t.min, t.hour, t.wday, t.mday, t.mon, t.year_s };

    Wire.beginTransmission(DS3231_I2C_ADDR);
    Wire.write(index);
        TimeDate[index] = dectobcd(TimeDate[index]);
        if (index == 5){TimeDate[5] += century;}
        Wire.write(TimeDate[index]);
    Wire.endTransmission();
    
    //Adjust the month setting, per data sheet, if the year is changed
    if (index == 6){
      Wire.beginTransmission(DS3231_I2C_ADDR);
      Wire.write(5);
      TimeDate[5] = dectobcd(TimeDate[5]);
      TimeDate[5] += century;
      Wire.write(TimeDate[5]);
      Wire.endTransmission();
    } 
}
