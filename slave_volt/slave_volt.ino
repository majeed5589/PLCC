#define ANALOG_IN_PIN A3

float adc_voltage = 0.0;
float in_voltage = 0.0;

// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0;

// Float for Reference Voltage
float ref_voltage = 5.0;

int adc_value = 0;

void setup() {
  Serial.begin(9600); // Start serial communication
}

void loop() {
  // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);
  
  // Determine voltage at ADC input
  adc_voltage = (adc_value * ref_voltage) / 1024.0;
  
  // Calculate voltage at divider input
  in_voltage = adc_voltage * (R1 + R2) / R2;

  // Send voltage data with proper delimiters
  Serial.print("<V:"); // Prefix for voltage data
  Serial.print(in_voltage, 2); // Voltage data with 2 decimal places
  Serial.println(">"); // End delimiter

  // Short delay
  delay(3000);
}
