#ifndef DHT_H_
#define DHT_H_

int8_t DHT_getTemperature(float *temp, int8_t typ);
int8_t DHT_getHumidity(float *hum, int8_t typ);
int8_t DHT_getTemperatureHumidity(float *temp, float *hum, int8_t typ);

#endif
