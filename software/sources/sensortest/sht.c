#include "common.h"
#include "util/delay.h"
#include "sht.h"

void SHT_transmitInit() {
  SHT_DDR |= (1 << SHT_DATA);
  SHT_DDR |= (1 << SHT_SCK);
  SHT_PORT |= (1 << SHT_DATA);
  _delay_us(2);
  SHT_PORT |= (1 << SHT_SCK);
  _delay_us(2);
  SHT_PORT &= ~(1 << SHT_DATA);
  _delay_us(2);
  SHT_PORT &= ~(1 << SHT_SCK);
  _delay_us(1);
  SHT_PORT |= (1 << SHT_SCK);
  _delay_us(2);
  SHT_PORT |= (1 << SHT_DATA);
  _delay_us(2);
  SHT_PORT &= ~(1 << SHT_SCK);
}

void SHT_sendCommand(uint8_t cmd) {
  _delay_us(4);
  SHT_DDR |= (1 << SHT_DATA);
  for (int i = 0; i < 8; i++) {
    if ((cmd >> (7 - i)) & 0x01) {
      SHT_PORT |= (1 << SHT_DATA);
      } else {
      SHT_PORT &= ~(1 << SHT_DATA);
    }
    _delay_us(4);
    SHT_PORT |= (1 << SHT_SCK);
    _delay_us(4);
    SHT_PORT &= ~(1 << SHT_SCK);
    if (i == 7) {
      SHT_PORT |= (1 << SHT_DATA);
      SHT_DDR &= ~(1 << SHT_DATA);
      long timeoutCounter = 0;
      while (SHT_PIN & (1 << SHT_DATA)) {
        if (timeoutCounter++ > 400000) break;
        if (!(TEST_PIN & (1 << TEST_INPUT))) break;
      }
    }
  }
  _delay_us(4);
  SHT_PORT |= (1 << SHT_SCK);
  _delay_us(4);
  SHT_PORT &= ~(1 << SHT_SCK);
}

void SHT_reset() {
  SHT_DDR |= (1 << SHT_DATA);
  SHT_PORT |= (1 << SHT_DATA);
  for (int i = 0; i < 9; i++) {
    _delay_us(4);
    SHT_PORT |= (1 << SHT_SCK);
    _delay_us(4);
    SHT_PORT &= ~(1 << SHT_SCK);
  }
  SHT_transmitInit();
}

uint16_t SHT_readData() {
  uint8_t data_l, data_h;
  uint16_t measured;
  data_l = 0x00;
  data_h = 0x00;
  SHT_PORT |= (1 << SHT_DATA);
  SHT_DDR &= ~(1 << SHT_DATA);
  long timeoutCounter = 0;
  while (SHT_PIN & (1 << SHT_DATA)) {
    if (timeoutCounter++ > 400000) break;
    if (!(TEST_PIN & (1 << TEST_INPUT))) break;
  }
  SHT_receiveBytes(&data_h);
  SHT_ack();
  SHT_receiveBytes(&data_l);
  measured = (data_h << 8) | data_l;
  return measured;
}

float SHT_calculateMeasuredData(uint16_t measuredData, uint8_t val) {
  switch (val) {
    case 0x05:
    return -4.0 + 0.0405 * measuredData - 2.8 * pow(10, -6) * pow(measuredData, 2);
    break;
    
    case 0x03:
    return -40.01 + 0.01 * measuredData;
    break;
  }
  return 1;
}

void SHT_sense(float *temp, float *hum) {
  float _hum;
  SHT_transmitInit();
  SHT_sendCommand(0x03);
  _delay_us(80);
  *temp = SHT_calculateMeasuredData(SHT_readData(), 0x03);
  SHT_reset();
  SHT_sendCommand(0x05);
  _delay_us(80);
  _hum = SHT_readData();
  *hum = SHT_calculateMeasuredData(_hum, 0x05);
  *hum = (*temp - 25) * (0.01 + 0.00008 * _hum) + *hum;
  SHT_reset();
  for (int i=0; i < 5; i++) {
    if (!(TEST_PIN & (1 << TEST_INPUT))) break;
    _delay_ms(20);
  }
}

void SHT_receiveBytes(uint8_t *target) {
  for (int i = 0; i < 8; i++) {
    SHT_PORT |= (1 << SHT_SCK);
    _delay_us(4);
    if (SHT_PIN & (1 << SHT_DATA)) {
      *target |= (1 << (7 - i));
      } else {
      *target &= ~(1 << (7 - i));
    }
    SHT_PORT &= ~(1 << SHT_SCK);
    _delay_us(4);
  }
}

void SHT_ack(void) {
  SHT_DDR |= (1 << SHT_DATA);
  SHT_PORT &= ~(1 << SHT_DATA);
  _delay_us(4);
  SHT_PORT |= (1 << SHT_SCK);
  _delay_us(4);
  SHT_PORT &= ~(1 << SHT_SCK);
  _delay_us(4);
  SHT_PORT |= (1 << SHT_DATA);
  SHT_DDR &= ~(1 << SHT_DATA);
}
