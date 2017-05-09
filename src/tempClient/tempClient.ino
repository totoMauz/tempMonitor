#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define PIN_DHT                  D3
#define PIN_SWITCH               D0


//DHT11 can only read every 2 s.
#define INTERVAL_DHT_READ      2000
#define INTERVAL_DISPLAY_DIM   3000

DHT dht(PIN_DHT, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

uint32_t timeSwitchReleased;
uint32_t timeDhtRead;
int temp;
int humi;
bool switchStateOld;

void readDht() {
  if ((timeDhtRead + INTERVAL_DHT_READ) <= millis()) {
    timeDhtRead = millis();
    if (dht.read()) {
      humi = dht.readHumidity();
      temp = dht.readTemperature();
    }
  }
}

void updateDisplay() {
  bool isSwitchPressed = digitalRead(PIN_SWITCH) == HIGH;

  if (isSwitchPressed) {
    timeSwitchReleased = millis();
  }

  if (isSwitchPressed || timeSwitchReleased + INTERVAL_DISPLAY_DIM > millis()) {
    lcd.backlight();
  }
  else {
    lcd.noBacklight();
  }

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
  pinMode(D0, INPUT);
}

void loop()
{
  readDht();
  updateDisplay();
  delay(100);
}
