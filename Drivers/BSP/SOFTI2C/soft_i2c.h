#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include "main.h"
#include "cmsis_os.h"

typedef struct {
    GPIO_TypeDef *SCL_Port;
    uint16_t SCL_GPIO_Pin;
    GPIO_TypeDef *SDA_Port;
    uint16_t SDA_GPIO_Pin;
    osMutexId_t mutex;
} SoftI2C_HandleTypeDef;

void SoftI2C_Init(SoftI2C_HandleTypeDef *hi2c);
void SoftI2C_Start(SoftI2C_HandleTypeDef *hi2c);
void SoftI2C_Stop(SoftI2C_HandleTypeDef *hi2c);
uint8_t SoftI2C_WriteByte(SoftI2C_HandleTypeDef *hi2c, uint8_t data);
uint8_t SoftI2C_ReadByte(SoftI2C_HandleTypeDef *hi2c, uint8_t ack);

#endif
