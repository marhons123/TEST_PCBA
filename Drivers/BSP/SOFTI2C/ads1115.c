#include "ads1115.h"
#include "soft_i2c.h"
#include "stm32f4xx_hal.h"   // 根据你MCU型号调整

// 写寄存器，写入16位值
void ADS1115_WriteRegister(SoftI2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t value) {
    SoftI2C_Start(hi2c);
    SoftI2C_WriteByte(hi2c, ADS1115_ADDR);       // 写器件地址
    SoftI2C_WriteByte(hi2c, reg);                // 写寄存器地址
    SoftI2C_WriteByte(hi2c, value >> 8);         // 写高字节
    SoftI2C_WriteByte(hi2c, value & 0xFF);       // 写低字节
    SoftI2C_Stop(hi2c);
}

// 读寄存器，读取16位值
uint16_t ADS1115_ReadRegister(SoftI2C_HandleTypeDef *hi2c, uint8_t reg) {
    uint16_t value;
    SoftI2C_Start(hi2c);
    SoftI2C_WriteByte(hi2c, ADS1115_ADDR);
    SoftI2C_WriteByte(hi2c, reg);
    SoftI2C_Start(hi2c);
    SoftI2C_WriteByte(hi2c, ADS1115_ADDR | 0x01);   // 读地址
    uint8_t high = SoftI2C_ReadByte(hi2c, 1);
    uint8_t low = SoftI2C_ReadByte(hi2c, 0);
    SoftI2C_Stop(hi2c);

    value = (high << 8) | low;
    return value;
}

// 读取单通道ADC值，启动单次转换，通道AIN0单端输入
int16_t ADS1115_ReadSingleChannel(SoftI2C_HandleTypeDef *hi2c) {
    // 配置寄存器设置：启动单次转换，AIN0单端，FS=±4.096V，128SPS
    uint16_t config = 0x8483;
    ADS1115_WriteRegister(hi2c, ADS1115_REG_CONFIG, config);

    HAL_Delay(10);  // 等待转换完成，根据采样率调整延时

    uint16_t raw = ADS1115_ReadRegister(hi2c, ADS1115_REG_CONVERT);
    return (int16_t)raw;  // 转换为带符号数返回
}

float ADS1115_RawToVoltage(int16_t raw, float pga_range) {
    return (float)raw * pga_range / 32768.0f;
}
// 测试函数，初始化I2C并读取一次数据打印
void ADS1115_Test(void) {
    SoftI2C_HandleTypeDef hi2c_ads = {
        GPIOH, GPIO_PIN_4,   // SDA
        GPIOH, GPIO_PIN_5,   // SCL
        NULL
    };
    SoftI2C_Init(&hi2c_ads);

//    printf(">> ADS1115 Test Start\r\n");

    int16_t value = ADS1115_ReadSingleChannel(&hi2c_ads);
//    printf("ADS1115 Channel 0 raw value: %d\r\n", value);

    float voltage = ADS1115_RawToVoltage(value, 4.096f);  // ±4.096V 量程
//    printf("ADS1115 Channel 0 voltage: %.3f V\r\n", voltage);

//    printf(">> ADS1115 Test End\r\n");
}


