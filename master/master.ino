#include <Wire.h>
#include <LiquidCrystal_I2C.h>

String data = "";
bool receiving = false;
float temp_rx = 0.0;
float humidity_rx = 0.0;
float volt_rx = 0.0;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  while (Serial1.available() > 0) {
    char receivedChar = Serial1.read();
    
    // Start marker detection
    if (receivedChar == '<') {
      data = "";
      receiving = true;
    } 
    // End marker detection
    else if (receivedChar == '>') {
      receiving = false;
      parseData(data); // Parse the complete string
    } 
    // Collect data between markers
    else if (receiving) {
      data += receivedChar;
    }
  }
  delay(100);
}

void parseData(String receivedData) {
  if (receivedData.startsWith("T:")) {
    int tempIndex = receivedData.indexOf(':') + 1;
    int humidityIndex = receivedData.indexOf(",H:");

    if (humidityIndex > -1) {
      String tempStr = receivedData.substring(tempIndex, humidityIndex);
      String humidityStr = receivedData.substring(humidityIndex + 3);

      if (isValidFloat(tempStr) && isValidFloat(humidityStr)) {
        temp_rx = tempStr.toFloat();
        humidity_rx = humidityStr.toFloat();
        
        Serial.print("Temperature: ");
        Serial.println(temp_rx);
        Serial.print("Humidity: ");
        Serial.println(humidity_rx);

        // Update LCD display
        lcd.setCursor(0, 0);
        lcd.print("T: ");
        lcd.print(temp_rx);
        lcd.setCursor(0, 1);
        lcd.print("H: ");
        lcd.print(humidity_rx);
      }
    }
  } else if (receivedData.startsWith("V:")) {
    int voltIndex = receivedData.indexOf(':') + 1;
    String voltStr = receivedData.substring(voltIndex);

    if (isValidFloat(voltStr)) {
      volt_rx = voltStr.toFloat();
      
      Serial.print("Voltage: ");
      Serial.println(volt_rx);

      // Update LCD display
      lcd.setCursor(8, 1);  // Display voltage on the second line, starting from the 9th character
      lcd.print("V: ");
      lcd.print(volt_rx);
    } else {
      Serial.println("Invalid Voltage Data");
    }
  }
}

bool isValidFloat(String str) {
  bool hasDecimal = false;
  int len = str.length();
  if (len == 0) return false;
  for (int i = 0; i < len; i++) {
    if (i == 0 && str[i] == '-') continue; // Allow negative sign at the beginning
    if (str[i] == '.') {
      if (hasDecimal) return false; // More than one decimal point
      hasDecimal = true;
    } else if (!isDigit(str[i])) {
      return false; // Non-digit character
    }
  }
  return true;
}
