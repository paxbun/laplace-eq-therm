// https://github.com/matmunk/DS18B20
#include <DS18B20.h>

// DAT -> 2
// VCC -> 3
// GND -> 4
DS18B20 ds(2);

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
}

void loop() {
  while (ds.selectNext()) {
    Serial.println(ds.getTempC());
  }
}