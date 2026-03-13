#ifndef __AT24C02_H__
#define __AT24C02_H__

#include "soft_i2c.h"

#define AT24C02_ADDR 0xA0  // ¹Ì¶¨Æ÷¼þµØÖ·

uint8_t AT24C02_WriteByte(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t data);
uint8_t AT24C02_ReadByte(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *data);
uint8_t AT24C02_WriteBytes(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *data, uint8_t len);
uint8_t AT24C02_ReadBytes(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *buf, uint8_t len);
void AT24C02_Test(void);
#endif
