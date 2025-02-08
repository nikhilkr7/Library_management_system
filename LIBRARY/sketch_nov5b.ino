#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11      // DHT 11
#define MQ135_PIN A0       // Analog pin connected to the MQ135 sensor

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 or 0x3F

// Frame control variables
unsigned long previousMillis = 0;
const long interval = 3000; // Change frame every 3 seconds
bool showFirstFrame = true;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();            // Initialize the LCD
  lcd.backlight();       // Turn on the LCD backlight
  lcd.clear();
  lcd.print("Air Quality");
  delay(2000);           // Display startup message
  lcd.clear();
}

// Function to simulate AQI in a stable range
int simulateAQI() {
  int baseAQI = 53;                 // Base AQI level
  int randomFluctuation = random(-1, 2);  // Minor fluctuation (-1 to +1)
  return baseAQI + randomFluctuation;     // Stable AQI value with small variance
}

// Function to get air quality status based on AQI, temperature, and humidity
String getAirQualityStatus(int aqi, float temperature, float humidity) {
  if (aqi <= 50 && temperature <= 25 && humidity <= 60) {
    return "Good";
  } else if (aqi <= 55 && (temperature > 25 || humidity > 60)) {
    return "Moderate";
  } else {
    return "Bad";
  }
}

void loop() {
  int aqi = simulateAQI();
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  float humidity = dht.readHumidity();       // Read humidity

  // Check if DHT sensor data is available
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Determine air quality status
  String airQualityStatus = getAirQualityStatus(aqi, temperature, humidity);

  // Switch between frames every 3 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    showFirstFrame = !showFirstFrame; // Toggle frame
  }

  lcd.clear();

  if (showFirstFrame) {
    // Frame 1: Display AQI, Temperature, and Humidity
    lcd.setCursor(0, 0);
    lcd.print("AQI:");
    lcd.print(aqi);            // Display AQI
    lcd.setCursor(7, 0);
    lcd.print("Temp:");
    lcd.print(temperature, 1);  // Display temperature with one decimal
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print(humidity, 1);     // Display humidity with one decimal
    lcd.print("%");
  } else {
    // Frame 2: Display Air Quality Status
    lcd.setCursor(0, 0);
    lcd.print("Air Quality:");
    lcd.setCursor(0, 1);
    lcd.print(airQualityStatus); // Display air quality status
    delay(500);
  }
  
  // Print values on Serial Monitor (optional for debugging)
  Serial.print("AQI: ");
  Serial.print(aqi);
  Serial.print(", Status: ");
  Serial.print(airQualityStatus);
  Serial.print(", Temp: ");
  Serial.print(temperature);
  Serial.print("C, Hum: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(800);  // Short delay to prevent rapid screen flickering
}