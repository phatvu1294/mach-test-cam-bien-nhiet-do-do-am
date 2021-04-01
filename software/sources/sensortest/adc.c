#include "common.h"
#include "adc.h"

uint16_t ADC_read(uint8_t chanel) {
	ADMUX = ADC_VREF_AVCC | chanel;
	ADCSRA = (1 << ADEN) | (1 << ADSC) | ADC_PRESCALER_128;
	while (!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF);
	return ADC;
}

float ADC_getMeans(uint16_t times, uint8_t chanel) {
	uint32_t sum = 0;
	float means = 0;
	for (int i = 0; i < times; i++) {
		sum += ADC_read(chanel);
	}
	means = (float)sum / times;
	return means;
}

long ADC_getResistence(uint16_t adcValue, uint16_t adcBalanceResistor) {
	if (adcValue == 0)
		return 0;
	else
		return (long)((long)(1023 * (long)adcBalanceResistor) / adcValue - (long)adcBalanceResistor);
}
