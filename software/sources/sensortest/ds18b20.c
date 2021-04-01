#include "common.h"
#include <util/delay.h>
#include "DS18B20.h"

uint8_t DS18B20_reset() {
  uint8_t i;
  DS18B20_PORT &= ~(1 << DS18B20_INPUT);
  DS18B20_DDR |= (1 << DS18B20_INPUT);
  _delay_us(480);
  DS18B20_DDR &= ~(1 << DS18B20_INPUT);
  _delay_us(60);
  i = (DS18B20_PIN & (1 << DS18B20_INPUT));
  _delay_us(420);
  return i;
}

void DS18B20_writeBit(uint8_t bit) {
  DS18B20_PORT &= ~(1 << DS18B20_INPUT);
  DS18B20_DDR |= (1 << DS18B20_INPUT);
  _delay_us(1);
  if (bit)
  DS18B20_DDR &= ~(1 << DS18B20_INPUT);
  _delay_us(60);
  DS18B20_DDR &= ~(1 << DS18B20_INPUT);
}

uint8_t DS18B20_readBit(void) {
  uint8_t bit = 0;
  DS18B20_PORT &= ~ (1 << DS18B20_INPUT);
  DS18B20_DDR |= (1 << DS18B20_INPUT);
  _delay_us(1);
  DS18B20_DDR &= ~(1 << DS18B20_INPUT);
  _delay_us(14);
  if (DS18B20_PIN & (1 << DS18B20_INPUT))
  bit = 1;
  _delay_us(45);
  return bit;
}

void DS18B20_writeByte(uint8_t byte) {
  uint8_t i = 8;
  while (i--) {
    if (!(TEST_PIN & (1 << TEST_INPUT))) break;
    DS18B20_writeBit(byte & 0x01);
    byte >>= 1;
  }
}

uint8_t DS18B20_readByte(void) {
  uint8_t i = 8, n = 0;
  while (i--) {
    n >>= 1;
    n |= (DS18B20_readBit() << 7);
  }
  return n;
}

float DS18B20_getTemp() {
  uint8_t temperature_l;
  uint8_t temperature_h;
  uint16_t temperature_d;
  float temperature_c = 0;
  DS18B20_reset();
  DS18B20_writeByte(DS18B20_CMD_SKIPROM);
  DS18B20_writeByte(DS18B20_CMD_CONVERTTEMP);
  while (!DS18B20_readBit()) {
    if (!(TEST_PIN & (1 << TEST_INPUT))) break;
  }
  DS18B20_reset();
  DS18B20_writeByte(DS18B20_CMD_SKIPROM);
  DS18B20_writeByte(DS18B20_CMD_RSCRATCHPAD);
  temperature_l = DS18B20_readByte();
  temperature_h = DS18B20_readByte();
  temperature_d = (temperature_h << 8) | temperature_l;
  if ((temperature_d & 0xF800) == 0xF800) {
    temperature_c = (0xFFFF - temperature_d + 1)  * 0.0625 * (-1.0);
    } else {
    temperature_c = temperature_d * 0.0625;
  }
  return temperature_c;
}
