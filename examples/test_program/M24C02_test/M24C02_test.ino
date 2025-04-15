/** M24C02 operation sample
 *
 *  Test code
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 */

#include <string.h>
#include "M24C02.h"

M24C02 eeprom;

constexpr int buffer_size = 128;
constexpr char test_str0[] = "Hello, M24C02! This is test code for M24C02 class driver. Trying to write and read.";
char buffer[buffer_size];

int arbitraly_length_write_read(const char *w, int length);
int offset_write_read(void);
int offset_write_read_arbitrary_addr_long_data(void);
int single_write(void);
int single_read(void);
int update(void);
char *str0(char *b, int start, int end);
void show_data(uint8_t *dp, int data_length);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, M24C02! *****");
}

void loop() {
  uint32_t r = 0;

  r |= arbitraly_length_write_read(test_str0, sizeof(test_str0));
  r <<= 1;
  r |= arbitraly_length_write_read(str0(buffer, 64, 91), (91 - 64) + 1);
  r <<= 1;
  r |= arbitraly_length_write_read(str0(buffer, 64, 127), (127 - 64) + 1);
  r <<= 1;
  r |= arbitraly_length_write_read(str0(buffer, 126, 63), (127 - 64) + 1);
  r <<= 1;
  r |= arbitraly_length_write_read(str0(buffer, 48, 58), (58 - 48) + 1);
  r <<= 1;
  r |= arbitraly_length_write_read(str0(buffer, 57, 47), (58 - 48) + 1);
  r <<= 1;
  r |= offset_write_read();
  r <<= 1;
  r |= offset_write_read_arbitrary_addr_long_data();
  r <<= 1;
  r |= single_write();
  r <<= 1;
  r |= single_read();
  r <<= 1;
  r |= update();
  r <<= 1;


  Serial.print("OVERALL RESULT : ");
  Serial.println(r ? "FAIL" : "PASS");
  Serial.println("");

  delay(300000);
}

int arbitraly_length_write_read(const char *w, int length) {
  char r[length];

  Serial.print("*** arbitraly length write/read test. write/read length = ");
  Serial.println(length);

  Serial.print("write :");
  Serial.println(w);

  eeprom.write(0, (uint8_t *)w, length);
  eeprom.read(0, (uint8_t *)r, length);

  Serial.print("read  :");
  Serial.println(r);

  Serial.print("result:");

  int rtn = strcmp(w, r) ? 1 : 0;
  Serial.println(rtn ? "FAIL" : "PASS");

  return rtn;
}

int offset_write_read(void) {
  char r[buffer_size];

  Serial.println("*** offset_write_read");

  eeprom.write(0, (uint8_t *)"000000000000000000000000000000", 31);

  eeprom.read(0, (uint8_t *)r, 31);
  Serial.print("initial read :");
  Serial.println(r);

  eeprom.write(3, (uint8_t *)"ABC", 3);

  eeprom.read(0, (uint8_t *)r, 31);
  Serial.print("2nd read     :");
  Serial.println(r);

  eeprom.write(6, (uint8_t *)"DEF", 3);

  eeprom.read(0, (uint8_t *)r, 31);
  Serial.print("3rd read     :");
  Serial.println(r);

  if (strcmp(r, "000ABCDEF000000000000000000000")) {
    Serial.println("FAIL");
    return 1;
  }

  eeprom.read(3, (uint8_t *)r, 6);
  Serial.print("6 bytes fron addr=3:");
  Serial.println(r);

  if (strncmp(r, "ABCDEF", 6)) {
    Serial.println("FAIL");
    return 1;
  }


  Serial.println("PASS");
  return 0;
}

int offset_write_read_arbitrary_addr_long_data(void) {
  constexpr int length = 256;
  char cb[length];
  char rb[length];
  constexpr char test_str0[] = "Hello, M24C02! This is test code for M24C02 class driver. Trying to write and read.";

  Serial.println("*** offset_write_read_arbitrary_addr_long_data");

  memset(cb, '0', sizeof(cb));
  eeprom.write(0, (uint8_t *)cb, length);

  eeprom.read(0, (uint8_t *)rb, length);
  Serial.print("initial read :");
  Serial.println(rb);

  if (memcmp(rb, cb, length)) {
    Serial.println("FAIL");
    return 1;
  }


  eeprom.write(3, (uint8_t *)test_str0, sizeof(test_str0));
  memcpy(cb + 3, test_str0, sizeof(test_str0));

  eeprom.read(0, (uint8_t *)rb, length);
  Serial.print("2nd read     :");
  show_data((uint8_t *)rb, length);

  eeprom.write(125, (uint8_t *)test_str0, sizeof(test_str0));
  memcpy(cb + 125, test_str0, sizeof(test_str0));

  eeprom.read(0, (uint8_t *)rb, length);
  Serial.print("3rd read     :");
  show_data((uint8_t *)rb, length);

  if (memcmp(rb, cb, length)) {
    Serial.println("FAIL");
    return 1;
  }

  Serial.println("PASS");
  return 0;
}

int single_write(void) {
  char r[buffer_size];

  Serial.println("*** single_write");

  Serial.print("*** single_write test : ");

  for (int i = 0; i < buffer_size; i++)
    buffer[i] = 0;

  eeprom.write(0, (uint8_t *)buffer, buffer_size);

  for (int i = 0; i < 128; i++)
    eeprom.write(i, i);

  eeprom.read(0, (uint8_t *)r, buffer_size);

  for (int i = 0; i < 128; i++)
    if (i != r[i]) {
      Serial.println("FAIL");
      return 1;
    }


  for (int i = 0; i < 128; i++)
    eeprom.write(i, ~i);

  eeprom.read(0, (uint8_t *)r, buffer_size);

  for (int i = 0; i < 128; i++)
    if (i != ~r[i]) {
      Serial.println("FAIL");
      return 1;
    }

  Serial.println("PASS");
  return 0;
}

int single_read(void) {
  char r[buffer_size];

  Serial.println("*** single_read");

  Serial.print("*** single_read test : ");

  for (int i = 0; i < buffer_size; i++)
    buffer[i] = i;

  eeprom.write(0, (uint8_t *)buffer, buffer_size);

  for (int i = 0; i < 128; i++)
    r[i] = eeprom.read(i);

  for (int i = 0; i < 128; i++)
    if (i != r[i]) {
      Serial.println("FAIL");
      return 1;
    }


  for (int i = 0; i < buffer_size; i++)
    buffer[i] = ~i;

  eeprom.write(0, (uint8_t *)buffer, buffer_size);

  for (int i = 0; i < 128; i++)
    r[i] = eeprom.read(i);

  for (int i = 0; i < 128; i++)
    if (i != ~r[i]) {
      Serial.println("FAIL");
      return 1;
    }


  Serial.println("PASS");
  return 0;
}

int update(void) {
  char wb[27] = { 0 };
  char rb[27] = { 0 };
  int err;

  Serial.println("*** update");

  for (int i = 0; i < 26; i++)
    wb[i] = i + 'A';

  eeprom.write(0, (uint8_t *)wb, sizeof(wb));

  eeprom.read(0, (uint8_t *)rb, 26);
  Serial.println(rb);

  for (int i = 0; i < 26; i++) {
    err = eeprom.update(i, wb[i]);

    if (0 != err)
      return 1;
  }

  for (int i = 0; i < 26; i++)
    wb[i] = i + 'a';

  for (int i = 0; i < 26; i++) {
    err = eeprom.update(i, wb[i]);

    if (!(1 == err))
      return 1;
  }

  eeprom.read(0, (uint8_t *)rb, 26);
  Serial.println(rb);

  if (strncmp(wb, rb, 26)) {
    Serial.println("FAIL");
    return 1;
  }


  Serial.println("PASS");
  return 0;
}

char *str0(char *b, int start, int end) {
  int order = (start < end) ? +1 : -1;
  char *b0 = b;

  for (int i = start; i != end; i += order)
    *b++ = i;

  *b = 0;

  return b0;
}

void show_data(uint8_t *dp, int data_length) {
  char s[2] = { 0 };

  for (int i = 0; i < data_length; i++) {
    *s = *dp++;
    Serial.print(s);
  }

  Serial.println("");
}
