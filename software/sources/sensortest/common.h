#ifndef COMMON_H_
#define COMMON_H_

#include <avr/io.h>
#define F_CPU 16000000ul

#define LCD_RS_DDR        DDRD
#define LCD_RS_PORT       PORTD
#define LCD_RS            7

#define LCD_RW_DDR        DDRD
#define LCD_RW_PORT       PORTD
#define LCD_RW            6

#define LCD_EN_DDR        DDRD
#define LCD_EN_PORT       PORTD
#define LCD_EN            5

#define LCD_DDR           DDRD
#define LCD_PORT          PORTD
#define LCD_D4    	      3
#define LCD_D5    	      2
#define LCD_D6   	        1
#define LCD_D7    	      0

#define BLIGHT_DDR		    DDRC
#define BLIGHT_PORT	      PORTC
#define BLIGHT_INPUT      5

#define ADC_DDR           DDRC
#define LM35_INPUT		    0
#define NTC_INPUT		      1

#define DHT_DDR		        DDRC
#define DHT_PORT	        PORTC
#define DHT_PIN		        PINC
#define DHT_INPUT	        2

#define DS18B20_DDR       DDRC
#define DS18B20_PORT      PORTC
#define DS18B20_PIN       PINC
#define DS18B20_INPUT     3

#define SHT_DDR  	        DDRB
#define SHT_PORT 	        PORTB
#define SHT_PIN  	        PINB
#define SHT_SCK  	        1
#define SHT_DATA 	        2

#define MENU_DDR          DDRB
#define MENU_PORT         PORTB
#define MENU_PIN          PINB
#define MENU_INPUT        0

#define TEST_DDR          DDRC
#define TEST_PORT         PORTC
#define TEST_PIN          PINC
#define TEST_INPUT        4

#endif
