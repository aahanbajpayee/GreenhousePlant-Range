#include "main.h"
#include "sh1106.h"
#include "SH1106Font.h"

extern SPI_HandleTypeDef hspi1;
uint8_t displayBuffer[8][128];

#define BUFFER_SIZE 1024  // Size of the display buffer in bytes
uint8_t buffer[BUFFER_SIZE];  // Display buffer

void OLED_init(void) {

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	Send_Command(0xAE); /*display off*/
	Send_Command(0xD5); /*set osc division*/
	Send_Command(0x80);
	Send_Command(0xA8); /*multiplex ratio*/
	Send_Command(0x3F); /*duty = 1/64*/
	Send_Command(0xD3); /*set display offset*/
	Send_Command(0x00); /* 0x20 */
	Send_Command(0x40); /*set display start line*/
	Send_Command(0xA1); /*set segment remap*/
	Send_Command(0xC8); /*Com scan direction*/
	setContrast(0XBF);
	Send_Command(0xD9); /*set pre-charge period*/
	Send_Command(0x22); /*0x22*/
	Send_Command(0xdb); /*set vcomh*/
	Send_Command(0x40);
	Send_Command(0x32); /*0X30---0X33 set VPP 8V */
	Send_Command(0xA6); /*normal / reverse*/

	OLED_clear();
	HAL_Delay(1);

	}

void Send_Byte(uint8_t value, uint8_t state)
{
	if(state==DATA)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_SPI_Transmit(&hspi1, &value, 1, 100);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi1, &value, 1, 100);
	}

	}

void Send_Data(uint8_t data)
{
	Send_Byte(data,DATA);
	}

void Send_Command(uint8_t data)
{
	Send_Byte(data,COMMAND);
	}

void OLED_set_position(uint8_t row, uint8_t col) {
    Send_Command(0xB0 + row);            // Set page start address
    Send_Command(0x00 + (col & 0x0F));   // Set lower column address
    Send_Command(0x10 + ((col >> 4) & 0x0F));   // Set higher column address
}

void setContrast(unsigned char val)
{
	Send_Command(0x81);
	Send_Command(val);
	}

void printChar(char msg)
{
	Send_Data(0x00);
    for(int i=0;i<5;i++)
    {
    	Send_Data(ASCII[msg-0x20][i]);
    }
    Send_Data(0x00);
}

void printStr(const char *msg)
{
    while(*msg!='\0')
    {
        printChar(*msg);
        msg++;
    }
}
void OLED_clear(void) {
	Send_Command(0xAE);
    for (uint8_t i = 0; i < 64; i++) {
        OLED_set_position(i, 0);   // Set cursor position to start of each page
        for (uint8_t j = 0; j < 128; j++) {
        	Send_Data(0x00);  // Write 0x00 to each column on the page
        }
    }
    Send_Command(0xAF);
}

void updateDisplay(void) {
    // Assuming you have a display buffer array
    // Modify this part based on your actual buffer implementation
    for (uint8_t i = 0; i < 8; i++) {
        OLED_set_position(i, 0);
        for (uint8_t j = 0; j < 128; j++) {
            // Assuming your buffer is a 2D array named "displayBuffer"
            Send_Data(displayBuffer[i][j]);
        }
    }
}

