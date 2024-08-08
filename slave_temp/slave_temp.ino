#include <DHT11.h>
float temperature;
float humidity;
DHT11 dht11(2);

void setup() {
  Serial.begin(9600);
  dht11.setDelay(500);
}

void loop() {
  temperature = dht11.readTemperature();
  humidity = dht11.readHumidity();
  Serial.print("<T:");
  Serial.print(temperature,2);
  Serial.print(",H:");
  Serial.print(humidity);
  Serial.print(">");
  delay(3000);
}
