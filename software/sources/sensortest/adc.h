#ifndef ADC_H_
#define ADC_H_

#define ADC_PRESCALER_2 	  0
#define ADC_PRESCALER_4 	  2
#define ADC_PRESCALER_16 	  4
#define ADC_PRESCALER_32 	  5
#define ADC_PRESCALER_64  	6
#define ADC_PRESCALER_128   7

#define ADC_VREF_AREF 	    0
#define ADC_VREF_AVCC 	    64
#define ADC_VREF_MISC1 	    128
#define ADC_VREF_MISC2    	192

uint16_t ADC_read(uint8_t chanel);
float ADC_getMeans(uint16_t times, uint8_t chanel);
long ADC_getResistence(uint16_t adcValue, uint16_t adcBalanceResistor);

#endif
