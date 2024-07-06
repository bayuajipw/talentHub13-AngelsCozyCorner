#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#define trigPin 2
#define echoPin 3
#define buzzerPin 4
#define dhtPin 5
#define redLedPin 6
#define yellowLedPin 7

dht DHT;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  
  lcd.begin(20, 4); // 20 columns, 4 rows
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome Guardian!");
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
}


void loop() {
  // Read distance from HC-SR04
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  // Read temperature and humidity from DHT22
  int chk = DHT.read22(dhtPin);
  float temp = DHT.temperature;
  float hum = DHT.humidity;
  // Display temperature and humidity on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(hum);
  lcd.print("%");
  lcd.setCursor(0, 2);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");


  // Check if temperature or humidity is outside safe range
  if (temp < 20 || temp > 25 || hum < 40 || hum > 60) {
    digitalWrite(yellowLedPin, HIGH); // Turn on yellow LED
    digitalWrite(redLedPin, LOW);     // Turn off red LED
    noTone(buzzerPin);                // Turn off buzzer
  } else {
    digitalWrite(yellowLedPin, LOW);  // Turn off yellow LED
  }
  // Check if baby is too close to the edge of the bed
  if (distance < 20) {
    digitalWrite(redLedPin, HIGH);    // Turn on red LED
    tone(buzzerPin, 1000);            // Turn on buzzer
    lcd.setCursor(0, 3);
    lcd.print("Warning: Baby Fell!");
  } else {
    digitalWrite(redLedPin, LOW);     // Turn off red LED
    noTone(buzzerPin);                 // Turn off buzzer
    lcd.setCursor(0, 3);
    lcd.print("                       "); // Clear line on LCD
  }
  delay(1000);
}
