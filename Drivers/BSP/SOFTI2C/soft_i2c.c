#include "soft_i2c.h"
#include "cmsis_os.h"

#define I2C_DELAY_US 5

static void I2C_Delay(void) {
    for (volatile int i = 0; i < 50; i++);
}

static void SDA_Output(SoftI2C_HandleTypeDef *hi2c) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = hi2c->SDA_GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(hi2c->SDA_Port, &GPIO_InitStruct);
}

static void SDA_Input(SoftI2C_HandleTypeDef *hi2c) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = hi2c->SDA_GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(hi2c->SDA_Port, &GPIO_InitStruct);
}

void SoftI2C_Init(SoftI2C_HandleTypeDef *hi2c) {
    SDA_Output(hi2c);
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin, GPIO_PIN_SET);
}

void SoftI2C_Start(SoftI2C_HandleTypeDef *hi2c) {
    SDA_Output(hi2c);
    HAL_GPIO_WritePin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin, GPIO_PIN_RESET);
    I2C_Delay();
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_RESET);
}

void SoftI2C_Stop(SoftI2C_HandleTypeDef *hi2c) {
    SDA_Output(hi2c);
    HAL_GPIO_WritePin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin, GPIO_PIN_SET);
    I2C_Delay();
}

uint8_t SoftI2C_WriteByte(SoftI2C_HandleTypeDef *hi2c, uint8_t data) {
    SDA_Output(hi2c);
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        data <<= 1;
        HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_SET);
        I2C_Delay();
        HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_RESET);
        I2C_Delay();
    }
    SDA_Input(hi2c);
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_SET);
    I2C_Delay();
    uint8_t ack = HAL_GPIO_ReadPin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin);
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_RESET);
    return (ack == GPIO_PIN_RESET);
}

uint8_t SoftI2C_ReadByte(SoftI2C_HandleTypeDef *hi2c, uint8_t ack) {
    uint8_t data = 0;
    SDA_Input(hi2c);
    for (int i = 0; i < 8; i++) {
        data <<= 1;
        HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_SET);
        I2C_Delay();
        if (HAL_GPIO_ReadPin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin)) {
            data |= 0x01;
        }
        HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_RESET);
        I2C_Delay();
    }
    SDA_Output(hi2c);
    HAL_GPIO_WritePin(hi2c->SDA_Port, hi2c->SDA_GPIO_Pin, ack ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_SET);
    I2C_Delay();
    HAL_GPIO_WritePin(hi2c->SCL_Port, hi2c->SCL_GPIO_Pin, GPIO_PIN_RESET);
    return data;
}
