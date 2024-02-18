///*
// * htu21d.h
// *
// *  Created on: Aug 4, 2023
// *      Author: aahan
// */
//

#ifndef HTU21D_H_
#define HTU21D_H_

#include "stm32f4xx_hal.h"
#include "stdio.h"

#define HTU21D_ADDRESS 0x40

float Humid, Temp;

I2C_HandleTypeDef hi2c2;

uint8_t HTU21D_TRIGGER_TEMP_MEASURE_HOLD = 0xE3;
uint8_t HTU21D_TRIGGER_HUMD_MEASURE_HOLD = 0xE5;

void wakeUpHTU21D()
{
    // Soft reset command
    uint8_t reset_cmd = 0xFE;
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c2, HTU21D_ADDRESS << 1, &reset_cmd, sizeof(reset_cmd), HAL_MAX_DELAY);
    HAL_Delay(15);

    if (status != HAL_OK)
    {
    	    uint32_t timeout = HAL_GetTick() + 1000;
    	    while (status == HAL_BUSY && HAL_GetTick() < timeout)
    	    {
    	        status = HAL_I2C_GetState(&hi2c2);
    	    }
    }
}

void readTemperature()
{
    HAL_StatusTypeDef status;
    uint8_t data[3];

    status = HAL_I2C_Master_Transmit(&hi2c2, HTU21D_ADDRESS << 1, &HTU21D_TRIGGER_TEMP_MEASURE_HOLD, sizeof(HTU21D_TRIGGER_TEMP_MEASURE_HOLD), HAL_MAX_DELAY);
    HAL_Delay(15); // Give some time for measurement

    // Read temperature data
    status = HAL_I2C_Master_Receive(&hi2c2, HTU21D_ADDRESS << 1, data, sizeof(data), HAL_MAX_DELAY);

    uint16_t rawValue = ((uint16_t)data[0] << 8) | data[1];
    float temperature = (float)(rawValue * 175.72 / 65536.00) - 46.85;

    if (status == HAL_OK)
    {
        Temp = temperature;
    }
    else{
    	wakeUpHTU21D();
    }
//    HAL_Delay(50);
}

void readHumidity()
{
    HAL_StatusTypeDef status;
    uint8_t data[3];

    status = HAL_I2C_Master_Transmit(&hi2c2, HTU21D_ADDRESS << 1, &HTU21D_TRIGGER_HUMD_MEASURE_HOLD, sizeof(HTU21D_TRIGGER_HUMD_MEASURE_HOLD), HAL_MAX_DELAY);

    HAL_Delay(15);

    // Read humidity data
    status = HAL_I2C_Master_Receive(&hi2c2, HTU21D_ADDRESS << 1, data, sizeof(data), HAL_MAX_DELAY);

    uint16_t rawValue = ((uint16_t)data[0] << 8) | data[1];
    float humidity = (float)(rawValue * 125.0 / 65536.0) - 6.0;

    if (status == HAL_OK)
    {
        Humid = humidity;
    }
    else {
    	wakeUpHTU21D();
    }
//    HAL_Delay(50);
}



#endif
/* HTU21D_H_ */

