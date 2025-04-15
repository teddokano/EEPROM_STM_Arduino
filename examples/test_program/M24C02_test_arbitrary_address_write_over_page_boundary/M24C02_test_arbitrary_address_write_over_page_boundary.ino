/** M24C02 operation sample
 *
 *  Simple sample
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 */

#include "M24C02.h"
#include <string.h>

M24C02 eeprom;

constexpr int length = 256;

constexpr char test_str0[] = "Hello, M24C02! This is test code for M24C02 class driver. Trying to write and read.";
char clear0[length];
char s[length];

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, M24C02! *****");

  memset(clear0, '0', sizeof(clear0));
  clear0[sizeof(clear0) - 1] = '\0';
}

void show_data(uint8_t *dp, int data_length) {
  char s[2] = { 0 };

  for (int i = 0; i < data_length; i++) {
    *s = *dp++;
    Serial.print(s);
  }

  Serial.println("");
}


void loop() {
  static uint8_t address_offset = 0;

  Serial.print("trying write with address offset = ");
  Serial.println(address_offset);

  eeprom.write(0, (uint8_t *)clear0, sizeof(clear0));

  Serial.print("written into EEPROM    :");
  Serial.println(clear0);

  eeprom.read(0, (uint8_t *)s, sizeof(test_str0));

  Serial.print("read back from EEPROM  :");
  show_data((uint8_t *)s, length);


  eeprom.write(address_offset, (uint8_t *)test_str0, sizeof(test_str0));

  Serial.print("written into EEPROM    :");
  Serial.println(test_str0);

  eeprom.read(0, (uint8_t *)s, length);

  Serial.print("read back from EEPROM  :");
  show_data((uint8_t *)s, length);

  address_offset++;

  delay(1);
}
