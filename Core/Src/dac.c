/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dac.c
  * @brief   This file provides code for the configuration
  *          of the DAC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "dac.h"

/* USER CODE BEGIN 0 */
uint16_t dacValue = 0;
float dacOut1 = 0;
/* USER CODE END 0 */

DAC_HandleTypeDef hdac;

/* DAC init function */
void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspInit 0 */

  /* USER CODE END DAC_MspInit 0 */
    /* DAC clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* DAC interrupt Init */
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN DAC_MspInit 1 */

  /* USER CODE END DAC_MspInit 1 */
  }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

  if(dacHandle->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspDeInit 0 */

  /* USER CODE END DAC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC_CLK_DISABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

    /* DAC interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN DAC_MspDeInit 1 */

  /* USER CODE END DAC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#define DAC_REF_VOLTAGE 2.5f
#define DAC_RESOLUTION  4095.0f
#define VOLTAGE_GAIN    (60.0f / DAC_REF_VOLTAGE)  // 24x

// 补偿开关：1=启用补偿（默认），0=屏蔽补偿（测试真实值）
#define ENABLE_VOLTAGE_COMPENSATION 1

typedef struct {
    float input;     // 你当时设置的目标
    float measured;  // 实际测得的输出
} VoltageMap;
////1号仪器
//static const VoltageMap voltageMap[] = {
//    {1.0f, 0.72f}, {1.5f, 1.114f}, {2.0f, 1.624f}, {2.5f, 2.142f},
//    {3.0f, 2.66f}, {3.5f, 3.177f}, {4.0f, 3.705f}, {4.5f, 4.223f},
//    {5.0f, 4.74f}, {5.5f, 5.256f}, {6.0f, 5.77f}, {6.5f, 6.29f},
//    {7.0f, 6.81f}, {8.0f, 7.86f}, {9.0f, 8.89f}, {10.0f, 9.93f},
//    {11.0f, 10.97f}, {12.0f, 12.02f}, {14.0f, 14.09f}, {16.0f, 16.17f},
//    {18.0f, 18.24f}, {20.0f, 20.32f}, {22.0f, 22.38f}, {24.0f, 24.47f},
//    {26.0f, 26.54f}, {28.0f, 28.62f}, {30.0f, 30.69f}, {35.0f, 35.87f},
//    {40.0f, 41.06f}, {45.0f, 46.25f}, {50.0f, 51.41f}, {55.0f, 56.59f},
//    {60.0f, 61.78f}
//};
//2号仪器
static const VoltageMap voltageMap[] = {
    {1.0f, 0.657f}, {1.5f, 1.08f}, {2.0f, 1.6f}, {2.5f, 2.13f},
    {3.0f, 2.65f}, {3.5f, 3.20f}, {4.0f, 3.738f}, {4.5f, 4.265f},
    {5.0f, 4.79f}, {5.5f, 5.32f}, {6.0f, 5.85f}, {6.5f, 6.37f},
    {7.0f, 6.9f}, {8.0f, 7.96f}, {9.0f, 9.02f}, {10.0f, 10.07f},
    {11.0f, 11.13f}, {12.0f, 12.19f}, {14.0f, 14.3f}, {16.0f, 16.43f},
    {18.0f, 18.54f}, {20.0f, 20.64f}, {22.0f, 22.76f}, {24.0f, 24.88f},
    {26.0f, 26.99f}, {28.0f, 29.1f}, {30.0f, 31.22f}, {35.0f, 36.49f},
    {40.0f, 41.76f}, {45.0f, 47.05f}, {50.0f, 52.32f}, {55.0f, 57.59f},
    {60.0f, 62.88f}
};

#define MAP_SIZE (sizeof(voltageMap) / sizeof(VoltageMap))

/**
 * @brief 给定目标想要实际输出的电压，查表反推应该设定的 DAC 值
 * @param real_voltage 想让输出达到的实际电压
 * @retval DAC 数值（0~4095）
 */
uint32_t voltageToDAC(float real_voltage)
{
    // 屏蔽补偿：直接计算理论DAC值（无补偿，测试真实值）
    if (ENABLE_VOLTAGE_COMPENSATION == 0)
    {
        // 核心逻辑：目标实际电压 → 除以放大倍数 → 转DAC值（无任何补偿）
        float dac_voltage = real_voltage / VOLTAGE_GAIN;
        uint32_t dac_val = (uint32_t)((dac_voltage / DAC_REF_VOLTAGE) * DAC_RESOLUTION);
        // 边界保护，防止超出DAC量程
        return (dac_val > 4095) ? 4095 : dac_val;
    }

    // 启用补偿：原有查表插值逻辑（保留所有注释和逻辑）
    if (real_voltage <= voltageMap[0].measured)
        real_voltage = voltageMap[0].measured;
    if (real_voltage >= voltageMap[MAP_SIZE - 1].measured)
        real_voltage = voltageMap[MAP_SIZE - 1].measured;

    for (int i = 0; i < MAP_SIZE - 1; ++i)
    {
        float m0 = voltageMap[i].measured;
        float m1 = voltageMap[i + 1].measured;

        if (real_voltage >= m0 && real_voltage <= m1)
        {
            float in0 = voltageMap[i].input;
            float in1 = voltageMap[i + 1].input;

            // 插值计算目标设定电压（你该设置多少 DAC 对应的输入电压）
            float target_input_voltage = in0 + (real_voltage - m0) * (in1 - in0) / (m1 - m0);

            // 换算成 DAC 电压（除以24倍放大）
            float dac_voltage = target_input_voltage / VOLTAGE_GAIN;

            // 换算成 DAC 数值
            return (uint32_t)((dac_voltage / DAC_REF_VOLTAGE) * DAC_RESOLUTION);
        }
    }

    return 0;
}
/* USER CODE END 1 */
