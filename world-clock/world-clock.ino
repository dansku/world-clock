/*

 Dani's New Awesome Clock
 
 */

// LCD Config
#include "U8glib.h"
U8GLIB_ST7920_128X64_1X u8g(6,5,4,7);

// DHT22
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);



// Start Time
int second = 20, minute = 48, hour = 20;

char BRhours[3];

void setup(){

  // Start Serial
  Serial.begin(9600);
  Serial.println("Starting...");

  //DHT Start
  dht.begin();

}

void loop(){
  //Let' draw
  u8g.firstPage();  
  do { draw(); } 
  while( u8g.nextPage() );
  delay(50);
}

// Char for regular clock
char hr[2],  mins[2], secs[2];

// Char for Other Countries
char BRh[2], USh[2], FRh[2], FIh[2];

void draw(void) {
  
int BRdiff = 14;
int USdiff = 16;
int FRdiff = 10;
int FIdiff = 9;

int hourBR = 0;
int hourUS = 0;
int hourFI = 0;
int hourFR = 0;

  // Silly code to fake hour ---- 
  static unsigned long lastTick = 0; 
  if (millis() - lastTick >= 1000) {
    lastTick = millis();
    second++;
  }

  if (second >= 60) { minute++;  second = 0; } // reset seconds to zero 
  if (minute >=60) { hour++; minute = 0; }     // reset minutes to zero
  if (hour >=24) { hour=0; minute = 0; }       // reset minutes to zero
  
    // Time Melbourne
  int y = 4; // Move
  char hours[3] = {(hour / 10) + 48, (hour % 10) + 48, 0};
  char minutes[3] = {(minute / 10) + 48, (minute % 10) + 48, 0};
  char seconds[3] = {(second / 10) + 48, (second % 10) + 48, 0};
  
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.drawStr( 5+y, 15, hours);
  u8g.drawStr( 18+y, 15,":");
  u8g.drawStr( 24+y, 15, minutes);
  u8g.drawStr( 37+y, 15,":");
  u8g.drawStr( 43+y, 15, seconds);

  // Date
  u8g.drawStr( 7, 30,"22/02/15"); 
  
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
  if(hour-BRdiff < 1){ hourBR = hour + BRdiff; } else { hourBR = hour - BRdiff;  }
  char hoursBR[3] = {(hourBR / 10) + 48, (hourBR % 10) + 48, 0};
  u8g.drawStr( 25, 45, hoursBR);
  u8g.drawStr( 37, 45,":");
  u8g.drawStr( 42, 45, minutes);

  // USA
  if(hour-USdiff < 1){ hourUS = hour + USdiff; } else { hourUS = hour - USdiff;  }
  char hoursUS[3] = {(hourUS / 10) + 48, (hourUS % 10) + 48, 0};
  u8g.drawStr(  85, 45, hoursUS);
  u8g.drawStr(  97, 45,":");
  u8g.drawStr( 102, 45, minutes);
  
  // France
  if(hour-FRdiff < 1){ hourFR = hour + FRdiff; } else { hourFR = hour - FRdiff;  }
  char hoursFR[3] = {(hourFR / 10) + 48, (hourFR % 10) + 48, 0};
  u8g.drawStr(  85, 60, hoursFR);
  u8g.drawStr(  97, 60,":");
  u8g.drawStr( 102, 60, minutes);
  
  // Finland
  if(hour-FIdiff < 1){ hourFI = hour + FIdiff; } else { hourFI = hour - FIdiff;  }
  char hoursFI[3] = {(hourFI / 10) + 48, (hourFI % 10) + 48, 0};
  u8g.drawStr( 25, 60, hoursFI);
  u8g.drawStr( 37, 60,":");
  u8g.drawStr( 42, 60, minutes);

}




