#include "common.h"
#include <util/delay.h>
#include <string.h>
#include "lcd.h"

void LCD_pulseEnable() {
  LCD_EN_PORT &= ~(1 << LCD_EN);
  _delay_us(1);
  LCD_EN_PORT |= (1 << LCD_EN);
  _delay_us(1);
  LCD_EN_PORT &= ~(1 << LCD_EN);
  _delay_ms(1);
}

void LCD_write4Bits(uint8_t nb) {
  if (((nb >> 0) & 0x01) == 1) { LCD_PORT |= (1 << LCD_D4); } else { LCD_PORT &= ~(1 << LCD_D4); }
  if (((nb >> 1) & 0x01) == 1) { LCD_PORT |= (1 << LCD_D5); } else { LCD_PORT &= ~(1 << LCD_D5); }
  if (((nb >> 2) & 0x01) == 1) { LCD_PORT |= (1 << LCD_D6); } else { LCD_PORT &= ~(1 << LCD_D6); }
  if (((nb >> 3) & 0x01) == 1) { LCD_PORT |= (1 << LCD_D7); } else { LCD_PORT &= ~(1 << LCD_D7); }
  LCD_pulseEnable();
}

void LCD_command(uint8_t cmd) {
  LCD_RS_PORT &= ~(1 << LCD_RS);
  LCD_RW_PORT &= ~(1 << LCD_RW);
  LCD_write4Bits(cmd >> 4);
  LCD_write4Bits(cmd);
}

void LCD_data(uint8_t dt) {
  LCD_RS_PORT |= (1 << LCD_RS);
  LCD_RW_PORT &= ~(1 << LCD_RW);
  LCD_write4Bits(dt >> 4);
  LCD_write4Bits(dt);
}

void LCD_char(char chr) {
  LCD_data((uint8_t) chr);
}

void LCD_string(char * str) {
  uint8_t i = 0;
  while (str[i] != 0) {
    LCD_char(str[i]);
    i++;
  }
}

void LCD_setPos(uint8_t x, uint8_t y) {
  uint8_t firstChar[] = {0x80, 0xC0, 0x94, 0xD4};
  LCD_command(firstChar[y] + x);
}

void LCD_clear() {
  LCD_command(0x01);
  _delay_ms(2);
}

void LCD_init() {
  LCD_RS_DDR |= (1 << LCD_RS);
  LCD_RW_DDR |= (1 << LCD_RW);
  LCD_EN_DDR |= (1 << LCD_EN);
  LCD_DDR |= (1 << LCD_D4);
  LCD_DDR |= (1 << LCD_D5);
  LCD_DDR |= (1 << LCD_D6);
  LCD_DDR |= (1 << LCD_D7);
  _delay_ms(50);
  LCD_RS_PORT &= ~(1 << LCD_RS);
  LCD_EN_PORT &= ~(1 << LCD_EN);
  LCD_RW_PORT &= ~(1 << LCD_RW);
  LCD_write4Bits(0x03);
  _delay_ms(5);
  LCD_write4Bits(0x03);
  _delay_ms(5);
  LCD_write4Bits(0x03);
  _delay_ms(1);
  LCD_write4Bits(0x02);
  LCD_command(0x28);
  LCD_command(0x0C);
  LCD_clear();
  LCD_command(0x06);
}

void lcdCenter(uint8_t y, char * s) {
  uint8_t len = strlen(s);
  uint8_t x = (16 - len) / 2;
  for (uint8_t i = 0; i <= x; i++) LCD_char(' ');
  LCD_setPos(x, y);
  LCD_string(s);
  for (uint8_t i = 0; i < (16 - x - len); i++) LCD_char(' ');
}

void lcdRunning(uint8_t x, uint8_t y, uint8_t pos, uint8_t dir) {
  switch (pos) {
  case 1:
    LCD_setPos(x, y);
    LCD_string((dir) ? ">  " : "  <");
    break;
  case 2:
    LCD_setPos(x, y);
    LCD_string((dir) ? ">> " : " <<");
    break;
  case 3:
    LCD_setPos(x, y);
    LCD_string((dir) ? ">>>" : "<<<");
    break;
  case 4:
    LCD_setPos(x, y);
    LCD_string((dir) ? " >>" : "<< ");
    break;
  case 5:
    LCD_setPos(x, y);
    LCD_string((dir) ? "  >" : "<  ");
    break;
  default:
    LCD_setPos(x, y);
    LCD_string("   ");
  }
}
