#include "common.h"
#include <string.h>
#include <util/delay.h>
#include "dht.h"

int8_t DHT_getData(float *temp, float* hum, int8_t typ) {
  uint8_t bits[5];
  uint8_t i, j = 0;
  memset(bits, 0, sizeof(bits));
  DHT_DDR |= (1 << DHT_INPUT);
  DHT_PORT |= (1 << DHT_INPUT);
  for (int i=0; i < 5; i++) {
    if (!(TEST_PIN & (1 << TEST_INPUT))) break;
    _delay_ms(20);
  }
  DHT_PORT &= ~(1 << DHT_INPUT);
  if (typ == 0)
  _delay_ms(18);
  else if (typ == 1)
  _delay_ms(20);
  DHT_PORT |= (1 << DHT_INPUT);
  DHT_DDR &= ~(1 << DHT_INPUT);
  _delay_us(40);
  if ((DHT_PIN & (1 << DHT_INPUT))) {
    return -1;
  }
  _delay_us(80);
  if (!(DHT_PIN & (1 << DHT_INPUT))) {
    return -1;
  }
  _delay_us(80);
  for (j = 0; j < 5; j++) {
    uint8_t result = 0;
    for (i = 0; i < 8; i++) {
      while (!(DHT_PIN & (1 << DHT_INPUT))) {
        if (!(TEST_PIN & (1 << TEST_INPUT))) break;
      }
      _delay_us(30);
      if (DHT_PIN & (1 << DHT_INPUT))
      result |= (1 << (7 - i));
      while (DHT_PIN & (1 << DHT_INPUT)) {
        if (!(TEST_PIN & (1 << TEST_INPUT))) break;
      }
    }
    bits[j] = result;
  }
  DHT_DDR |= (1 << DHT_INPUT);
  DHT_PORT |= (1 << DHT_INPUT);
  for (int i=0; i < 5; i++) {
    if (!(TEST_PIN & (1 << TEST_INPUT))) break;
    _delay_ms(20);
  }
  if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
    if (typ == 0) {
      *temp = bits[2];
      *hum = bits[0];
      } else if (typ == 1) {
      uint16_t rawhum = bits[0] << 8 | bits[1];
      uint16_t rawtemp = bits[2] << 8 | bits[3];
      if (rawtemp & 0x8000) {
        *temp = (float)((rawtemp & 0x7FFF) / 10.0) * -1.0;
        } else {
        *temp = (float)(rawtemp) / 10.0;
      }
      *hum = (float)(rawhum) / 10.0;
    }
    return 0;
  }
  return -1;
}

int8_t DHT_getTemperature(float *temp, int8_t typ) {
  float hum = 0;
  return DHT_getData(temp, &hum, typ);
}

int8_t DHT_getHumidity(float *hum, int8_t typ) {
  float temp = 0;
  return DHT_getData(&temp, hum, typ);
}

int8_t DHT_getTemperatureHumidity(float *temp, float *hum, int8_t typ) {
  return DHT_getData(temp, hum, typ);
}
