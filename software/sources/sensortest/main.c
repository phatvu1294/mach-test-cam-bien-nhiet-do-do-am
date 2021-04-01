#include "common.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"
#include "dht.h"
#include "ds18b20.h"
#include "sht.h"
#include "adc.h"
#include "ntc.h"

#define MNU_DHT11       0
#define MNU_DHT22       1
#define MNU_LM35        2
#define MNU_DS18B20     3
#define MNU_SHT         4
#define MNU_NTC_10K     5
#define MNU_NTC_100K    6

uint8_t menu = 0;
uint8_t isTest = 0;
uint8_t cnt = 0;
float temperature = 0;
float humidity = 0;
char bufStr[17];

int main(void) {
  LCD_init();
  menu = 0;
  LCD_clear();
  LCD_setPos(0, 0);
  lcdCenter(0, "  DHT11  ");
  BLIGHT_DDR |= (1 << BLIGHT_INPUT);
  BLIGHT_PORT |= (1 << BLIGHT_INPUT);
  MENU_DDR &= ~(1 << MENU_INPUT);
  TEST_DDR &= ~(1 << TEST_INPUT);
  ADC_DDR &= ~((1 << LM35_INPUT) | (1 << NTC_INPUT));
  TCNT1 = 53036;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK &= ~(1 << TOIE1);
  cli();
  
  while (1) {
    if (!(MENU_PIN & (1 << MENU_INPUT))) {
      _delay_ms(20);
      if (!(MENU_PIN & (1 << MENU_INPUT))) {
        if (!isTest) {
          if (++menu > 6) menu = MNU_DHT11;
          switch (menu) {
            case MNU_DHT11:
            LCD_clear();
            lcdCenter(0, "  DHT11  ");
            break;
            
            case MNU_DHT22:
            LCD_clear();
            lcdCenter(0, " DHT21/22");
            break;
            
            case MNU_DS18B20:
            LCD_clear();
            lcdCenter(0, " DS18B20 ");
            break;
            
            case MNU_LM35:
            LCD_clear();
            lcdCenter(0, "   LM35  ");
            break;
            
            case MNU_SHT:
            LCD_clear();
            lcdCenter(0, " SHT1x/7x");
            break;
            
            case MNU_NTC_10K:
            LCD_clear();
            lcdCenter(0, "  NTC10K ");
            break;
            
            case MNU_NTC_100K:
            LCD_clear();
            lcdCenter(0, "  NTC100K");
            break;
          }
        }
      }
      while (!(MENU_PIN & (1 << MENU_INPUT)));
    }
    
    if (!(TEST_PIN & (1 << TEST_INPUT))) {
      _delay_ms(20);
      if (!(TEST_PIN & (1 << TEST_INPUT))) {
        if (isTest) {
          cli();
          TIMSK &= ~(1 << TOIE1); 
          isTest = 0;
          cnt = 0;
          lcdRunning(0, 0, cnt, 0);
          lcdRunning(13, 0, cnt, 1);
          lcdCenter(1, "                ");
          } else {
          lcdCenter(1, "...");
          isTest = 1;
          TIMSK |= (1 << TOIE1);
          sei();             
        }
      }
      while (!(TEST_PIN & (1 << TEST_INPUT)));
    }
  }
}

ISR (TIMER1_OVF_vect) {
  TCNT1 = 53036;
  if (isTest) {
    lcdRunning(0, 0, cnt, 0);
    lcdRunning(13, 0, cnt, 1);
    if (++cnt > 7) cnt = 0;
    
    if (cnt == 7) {
      switch (menu) {
        case MNU_DHT11:
        if (DHT_getTemperatureHumidity(&temperature, &humidity, 0) != -1) {
          dtostrf(temperature, 0, 2, bufStr);
          LCD_setPos(0, 1);
          LCD_string(bufStr);
          LCD_data(0xDF);
          LCD_string("C    ");
          dtostrf(humidity, 0, 2, bufStr);
          LCD_setPos(8, 1);
          LCD_string(bufStr);
          LCD_string("%    ");
        }
        break;
        
        case MNU_DHT22:
        if (DHT_getTemperatureHumidity(&temperature, &humidity, 1) != -1) {
          dtostrf(temperature, 0, 2, bufStr);
          LCD_setPos(0, 1);
          LCD_string(bufStr);
          LCD_data(0xDF);
          LCD_string("C    ");
          
          dtostrf(humidity, 0, 2, bufStr);
          LCD_setPos(8, 1);
          LCD_string(bufStr);
          LCD_string("%    ");
        }
        break;
        
        case MNU_DS18B20:
        temperature = DS18B20_getTemp();
        dtostrf(temperature, 0, 2, bufStr);
        LCD_setPos(0, 1);
        LCD_string(bufStr);
        LCD_data(0xDF);
        LCD_string("C    ");
        break;
        
        case MNU_LM35:
        temperature = ((float)ADC_getMeans(100, LM35_INPUT) * 500.0f) / 1023.0f;
        dtostrf(temperature, 0, 2, bufStr);
        LCD_setPos(0, 1);
        LCD_string(bufStr);
        LCD_data(0xDF);
        LCD_string("C    ");
        break;
        
        case MNU_SHT:
        SHT_sense(&temperature, &humidity);
        dtostrf(temperature, 0, 2, bufStr);
        LCD_setPos(0, 1);
        LCD_string(bufStr);
        LCD_data(0xDF);
        LCD_string("C    ");
        dtostrf(humidity, 0, 2, bufStr);
        LCD_setPos(8, 1);
        LCD_string(bufStr);
        LCD_string("%    ");
        break;
        
        case MNU_NTC_10K:
        temperature = NTC_getTemp(10000, 3950, ADC_getResistence(ADC_getMeans(100, NTC_INPUT), 10000));
        dtostrf(temperature, 0, 2, bufStr);
        LCD_setPos(0, 1);
        LCD_string(bufStr);
        LCD_data(0xDF);
        LCD_string("C    ");
        break;
        
        case MNU_NTC_100K:
        temperature = NTC_getTemp(100000, 3950, ADC_getResistence(ADC_getMeans(100, NTC_INPUT), 10000));
        dtostrf(temperature, 0, 2, bufStr);
        LCD_setPos(0, 1);
        LCD_string(bufStr);
        LCD_data(0xDF);
        LCD_string("C    ");
        break;
      }
    }
  }
}