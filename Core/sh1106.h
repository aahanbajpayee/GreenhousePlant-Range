

#ifndef INC_SH1106_H_
#define INC_SH1106_H_

#define COMMAND 0
#define DATA    1

void OLED_init(void);
void Send_Data(uint8_t data);
void Send_Command(uint8_t data);
void OLED_set_position(uint8_t row, uint8_t col);
void setContrast(unsigned char val);
void printChar(char msg);
void printStr(const char *msg);
void OLED_clear(void);
void updateDisplay(void);

#endif
