# EEPROM_STM_Arduino
EEPROM:M24C02 class driver and operation sample code for [Arduino](https://www.arduino.cc) 

## What is this?
EEPROM:M24C02 interface to write/read data with simple interface.  

```cpp
#include "M24C02.h"

M24C02 eeprom;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, M24C02! *****");
}

void loop() {
  constexpr static char test_str0[] = "Hello, M24C02! This is test code for M24C02 class driver. Trying to write and read.";
  static char s[ 128 ];

  eeprom.write(0, (uint8_t *)test_str0, sizeof(test_str0));	// EEPROM write

  Serial.print("written into EEPROM    :");
  Serial.println(test_str0);

  eeprom.read(0, (uint8_t *)s, sizeof(test_str0));          // EEPROM read

  Serial.print("read back from EEPROM  :");
  Serial.println(s);

  delay(1000);
}
```

