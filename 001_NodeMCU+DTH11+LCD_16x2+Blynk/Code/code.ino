#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE); // set the LCD address to 0x27 for a 16 chars and 2 line display


char auth[] = "autoCode_by_blynk"; //Enter the Auth code which was send by Blink
char ssid[] = "wifiName";  //Enter your WIFI Name
char pass[] = "wifiPassword";  //Enter your WIFI Password

#define DHTPIN 2          // Digital pin 4
#define DHTTYPE DHT11     // DHT 11

float h = 0.0f;
 float t = 0.0f; 

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  h = dht.readHumidity();
  t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);

  dht.begin();
 
//LCD display
 lcd.begin(16,2); // for 16 x 2 LCD module
  lcd.clear();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer

 lcd.clear();                          // clears the LCD
  lcd.setCursor(0, 0);                  // sets the cursor on the first line
  lcd.print("Humid :");               // prints the message Humidity: on the LCD
  lcd.setCursor(10, 0);                 // sets the cursor on the 10th column on the LCD
  lcd.print(h);                         // prints the humidity value on the LCD
  
  lcd.setCursor(0, 1);                  //sets the cursor on the bottom line on the LCD
  lcd.print("Temp. :");            //prints the message Temperature on the LCD
  lcd.setCursor(10, 1);                 //sets the cursor on the 13 column on the bottom
  lcd.print(t);                         //prints the temperature value on the lcd

  if (isnan(h) || isnan(t)) {                    //This if statement check to see if the DHT sensor is connected or disconnected
    Serial.println("Sensor is not connected");   //prints the message sensor is not connected on the serial monitor  
    lcd.clear();                                 // clears the lcd
    lcd.setCursor(0, 0);                         // sets the cursor on the first line on the lcd
    lcd.print("Sensor");                         // prints sensor on the lcd
    lcd.setCursor(0, 1);                         // sets the cursor on the bottom on the lcd
    lcd.print("Disconnected");                   // prints Disconnected on the lcd
    return;                                      // return from the if statement
  }

  delay(2000);                                  // wait for two seconds
}