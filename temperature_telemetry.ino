#include <OneWire.h>          // loading the temperature sensor communication library
#include <DallasTemperature.h>// loading a library to define a temperature sensor
#include <RTClib.h>           // loading the library for the RTC module
#include <SD.h>               // loading the SD and SPI libraries needed to communicate with the SD card module
#include <SPI.h>

float temperature = 0.0;
#define oneWirePin 12
#define CS_PIN 53
unsigned long c = 58900; // waiting variable for the next temperature measurement (1 minute)

OneWire MyWire(oneWirePin);
DallasTemperature DS1820(&MyWire);
RTC_DS3231 rtc;  // Create an instance of the RTC_DS3231 class

File datoteka;

void setup() {
  Serial.begin(115200);  // setting up a serial connection
  Serial.println("Start");

  // check if the SD card is connected and active
  if (!SD.begin(CS_PIN)) {
    Serial.println("Problem with SD card");
    return;
  }
  else {
    Serial.println("SD card is ready");
  }

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // adjust RTC to compile time

  DS1820.begin(); // start temperature reading
  DS1820.requestTemperatures(); // start requesting temperature
  temperature = DS1820.getTempCByIndex(0); // temperature in °C

  DateTime now = rtc.now(); // get the current date and time and store it in the DateTime variable 'now'

  /Serial.print(now.dayOfTheWeek()); 
  /Serial.print(" ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print(" -- ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  Serial.print("Temperature is: ");
  Serial.print(temperature);
  Serial.println(" °C");

  datoteka = SD.open("tel_temp.txt", FILE_WRITE); // create and open a file on the SD card
  // write the requested data to the file
  if (datoteka) {
    datoteka.print(now.dayOfTheWeek());
    datoteka.print(" ");
    datoteka.print(now.day());
    datoteka.print("/");
    datoteka.print(now.month());
    datoteka.print("/");
    datoteka.print(now.year());
    datoteka.print(" -- ");
    datoteka.print(now.hour());
    datoteka.print(":");
    datoteka.print(now.minute());
    datoteka.print(":");
    datoteka.println(now.second());
    datoteka.print("Temperature is: ");
    datoteka.print(temperature);
    datoteka.println(" °C");
    datoteka.close();
  }
  else {
    Serial.println("Error opening file");
  }
  delay(c);
}

void loop() {

int brojac = 1; // since one minute has already passed since the initial measurement, the counter initially has the value 1 
 
while (brojac < 1440){ 
 DS1820.requestTemperatures(); 
 temperatura = DS1820.getTempCByIndex(0); 
 DateTime now = rtc.now(); 
// if it is the beginning of the hour, we print the day and date along with the time and temperature
 if (now.minute() == 0){
  datoteka = SD.open("tel_temp.txt", FILE_WRITE);
  Serial.print(now.dayOfTheWeek());
  Serial.print(" ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print(" -- ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
  Serial.print("Temperature is: ");
  Serial.print(temperature);
  Serial.println(" °C");

  if (datoteka){
    datoteka.print(now.dayOfTheWeek());
    datoteka.print(" ");
    datoteka.print(now.day());
    datoteka.print("/");
    datoteka.print(now.month());
    datoteka.print("/");
    datoteka.print(now.year());
    datoteka.print(" -- ");
    datoteka.print(now.hour());
    datoteka.print(":");
    datoteka.print(now.minute());
    datoteka.print(":");
    datoteka.println(now.second());
    datoteka.print("Temperature is: ");
    datoteka.print(temperature);
    datoteka.println(" °C");
    datoteka.close();
  }
   else {
    Serial.println("Error opening file");
  }
 }
 else{
      datoteka = SD.open("tel_temp.txt", FILE_WRITE);
      Serial.print("Temperature is: ");
      Serial.print(temperature);
      Serial.println(" °C");

      if (datoteka){
        datoteka.print("Temperature is: ");
        datoteka.print(temperature);
        datoteka.println(" °C");
        datoteka.close();
      }
      else {
        Serial.println("Error opening file");
      }
    }
    brojac++; 
    delay(c); 
  }
  Serial.println("Measurement finished.");
  delay(1000); 
  while(1); 
}
