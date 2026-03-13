#include "at24c02.h"
#include <stdio.h> // 添加用于串口打印的库

SoftI2C_HandleTypeDef hi2c_eeprom;

uint8_t AT24C02_WriteByte(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t data) {
    SoftI2C_Start(hi2c);
    if (!SoftI2C_WriteByte(hi2c, AT24C02_ADDR)) return 1;
    if (!SoftI2C_WriteByte(hi2c, addr)) return 2;
    if (!SoftI2C_WriteByte(hi2c, data)) return 3;
    SoftI2C_Stop(hi2c);
    HAL_Delay(5);
    return 0;
}

uint8_t AT24C02_ReadByte(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *data) {
    SoftI2C_Start(hi2c);
    if (!SoftI2C_WriteByte(hi2c, AT24C02_ADDR)) return 1;
    if (!SoftI2C_WriteByte(hi2c, addr)) return 2;

    SoftI2C_Start(hi2c);
    if (!SoftI2C_WriteByte(hi2c, AT24C02_ADDR | 0x01)) return 3;

    *data = SoftI2C_ReadByte(hi2c, 0);
    SoftI2C_Stop(hi2c);
    return 0;
}

uint8_t AT24C02_WriteBytes(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *data, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        AT24C02_WriteByte(hi2c, addr + i, data[i]);
        HAL_Delay(5);
    }
    return 0;
}

uint8_t AT24C02_ReadBytes(SoftI2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *buf, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        if (AT24C02_ReadByte(hi2c, addr + i, &buf[i]) != 0)
            return 1;
    }
    return 0;
}


void AT24C02_Test(void)
{
    printf("\r\n>> AT24C02 Test Start\r\n");

    SoftI2C_HandleTypeDef hi2c_eeprom = {
        GPIOH, GPIO_PIN_4,
        GPIOH, GPIO_PIN_5,
        NULL
    };
    SoftI2C_Init(&hi2c_eeprom);

    // Single byte write & read
    uint8_t test_addr = 0x10;
    uint8_t test_value = 0x5A;

    printf("Writing 0x%02X to address 0x%02X...\r\n", test_value, test_addr);
    if (AT24C02_WriteByte(&hi2c_eeprom, test_addr, test_value) == 0)
        printf("Write success\r\n");
    else {
        printf("Write failed\r\n");
        return;
    }

    HAL_Delay(10);

    uint8_t read_value = 0;
    if (AT24C02_ReadByte(&hi2c_eeprom, test_addr, &read_value) == 0) {
        printf("Read 0x%02X from address 0x%02X\r\n", read_value, test_addr);
        if (read_value == test_value)
            printf("Single byte test PASSED\r\n");
        else
            printf("Single byte test FAILED\r\n");
    } else {
        printf("Read failed\r\n");
    }

    // Multi-byte write & read
    uint8_t tx_data[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t rx_data[5] = {0};
    uint8_t start_addr = 0x20;

    printf("Writing multiple bytes to 0x%02X...\r\n", start_addr);
    if (AT24C02_WriteBytes(&hi2c_eeprom, start_addr, tx_data, sizeof(tx_data)) == 0)
        printf("Multi-byte write success\r\n");
    else {
        printf("Multi-byte write failed\r\n");
        return;
    }

    HAL_Delay(10);

    if (AT24C02_ReadBytes(&hi2c_eeprom, start_addr, rx_data, sizeof(rx_data)) == 0) {
        printf("Read multiple bytes from 0x%02X:\r\n", start_addr);
        int success = 1;
        for (int i = 0; i < sizeof(tx_data); i++) {
            printf(" Addr 0x%02X: Wrote 0x%02X, Read 0x%02X\r\n",
                        start_addr + i, tx_data[i], rx_data[i]);
            if (tx_data[i] != rx_data[i]) {
                success = 0;
            }
        }
        if (success)
            printf("Multi-byte test PASSED\r\n");
        else
            printf("Multi-byte test FAILED\r\n");
    } else {
        printf("Multi-byte read failed\r\n");
    }

    // Read after power cycle
    printf("Reading after power cycle from address 0x%02X...\r\n", test_addr);
    if (AT24C02_ReadByte(&hi2c_eeprom, test_addr, &read_value) == 0)
        printf("Read after power cycle: 0x%02X\r\n", read_value);
    else
        printf("Read after power cycle failed\r\n");

    printf(">> AT24C02 Test End\r\n");
}
