#ifndef SHT_H_
#define SHT_H_

void SHT_transmitInit();
void SHT_sendCommand(uint8_t cmd);
void SHT_reset();
uint16_t SHT_readData();
float SHT_calculateMeasuredData(uint16_t measuredData, uint8_t val);
void SHT_sense(float *temp, float *humd);
void SHT_receiveBytes(uint8_t *target);
void SHT_ack(void);

#endif
