#ifndef LCD_H_
#define LCD_H_

void LCD_pulseEnable();
void LCD_write4Bits(uint8_t nb);
void LCD_command(uint8_t cmd);
void LCD_data(uint8_t dt);
void LCD_char(char chr);
void LCD_string(char * str);
void LCD_setPos(uint8_t x, uint8_t y);
void LCD_clear();
void LCD_init();
void lcdCenter(uint8_t y, char * s);
void lcdRunning(uint8_t x, uint8_t y, uint8_t pos, uint8_t dir);

#endif
