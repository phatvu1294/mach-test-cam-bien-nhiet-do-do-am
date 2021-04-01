#ifndef DS18B20_H_
#define DS18B20_H_

#define DS18B20_CMD_CONVERTTEMP		0x44
#define DS18B20_CMD_RSCRATCHPAD  	0xBE
#define DS18B20_CMD_WSCRATCHPAD  	0x4E
#define DS18B20_CMD_CPYSCRATCHPAD	0x48
#define DS18B20_CMD_RECEEPROM    	0xB8
#define DS18B20_CMD_RPWRSUPPLY   	0xB4
#define DS18B20_CMD_SEARCHROM    	0xF0
#define DS18B20_CMD_READROM       	0x33
#define DS18B20_CMD_MATCHROM      	0x55
#define DS18B20_CMD_SKIPROM       	0xCC
#define DS18B20_CMD_ALARMSEARCH   	0xEC

uint8_t DS18B20_reset();
void DS18B20_writeBit(uint8_t bit);
uint8_t DS18B20_readBit(void);
void DS18B20_writeByte(uint8_t byte);
uint8_t DS18B20_readByte(void);
float DS18B20_getTemp();

#endif
