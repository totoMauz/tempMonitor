#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "DHT.h"
DHT dht(D3, DHT11);

uint32_t lastReadDht;
int temp;
int humi;

void readDht() {
  if ((lastReadDht + 2000) <= millis()) {
    //DHT11 can only read every 2 s.
    lastReadDht = millis();
    if (dht.read()) {
      humi = dht.readHumidity();
      temp = dht.readTemperature();
    }
  }
}

void writeToDisplay() {
  lcd.setCursor(0, 0);
  lcd.print(temp);
  lcd.print(" ");
  lcd.print((char) 223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print(humi);
  lcd.print(" %");
}

void setup()
{
  lcd.begin();
  dht.begin();
}

void loop()
{
  readDht();
  writeToDisplay();
}
