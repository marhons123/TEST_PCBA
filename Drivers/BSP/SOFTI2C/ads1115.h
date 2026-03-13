#ifndef __ADS1115_H__
#define __ADS1115_H__

#include "soft_i2c.h"
#include <stdint.h>

#define ADS1115_ADDR          0x90  // I2C写地址（7位地址左移1位）

// ADS1115寄存器地址
#define ADS1115_REG_CONVERT   0x00
#define ADS1115_REG_CONFIG    0x01

// 函数声明
void ADS1115_WriteRegister(SoftI2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t value);
uint16_t ADS1115_ReadRegister(SoftI2C_HandleTypeDef *hi2c, uint8_t reg);
int16_t ADS1115_ReadSingleChannel(SoftI2C_HandleTypeDef *hi2c);
void ADS1115_Test(void);

#endif
