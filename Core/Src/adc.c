/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
#include "stdbool.h"
#include "math.h"
#define VREF 2.5f // 参考电压
#define ADC_RESOLUTION 4095.0f // ADC分辨率
#define JUMP_CONFIRM_COUNT 3  // 连续跳变确认次数
//#define VREFINT_CAL (*(uint16_t*)0x1FFF75AA) // 内部参考电压校准值

// 定义变量
extern osSemaphoreId adcConversionCompleteSemaphoreHandle; // ADC转换完成信号量
uint32_t adc_buffer[ADC_BUFFER_SIZE * 3]; // 存储ADC采样数据
float voltageIn5 = 0;         			  // IN5通道电压
float voltageIn6 = 0;         			  // IN6通道电压
float temperature = 0;        			  // 内部温度
uint16_t value1 = 0;					  // ADC1 IN5采样值
uint16_t value2 = 0;					  // ADC1 IN6采样值
float alpha = 0.05;  					  // 为 alpha 赋一个适当的初始值

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the analog watchdog
  */
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.HighThreshold = 3000;
  AnalogWDGConfig.LowThreshold = 0;
  AnalogWDGConfig.ITMode = ENABLE;
  if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA5     ------> ADC1_IN5
    PA6     ------> ADC1_IN6
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA5     ------> ADC1_IN5
    PA6     ------> ADC1_IN6
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief 处理ADC采样数据
 * @param averageValueIn5 IN5通道的ADC平均值
 * @param averageValueVrefint 内部参考电压的ADC平均值
 * @param averageValueTemp 内部温度的ADC平均值
 */
void ProcessADCData(float averageValueIn5, float averageValueIn6,
		float averageValueTemp) {

	// 计算IN5通道的电压值
	value1 = averageValueIn5;
	voltageIn5 = averageValueIn5 * VREF / ADC_RESOLUTION;

	// 计算IN6通道的电压值
	value2 = averageValueIn6;
	voltageIn6 = averageValueIn6 * VREF / ADC_RESOLUTION;

	// 计算温度
	temperature = ((averageValueTemp * VREF / ADC_RESOLUTION) - 0.76) / 0.0025 + 25.0;
	// 首先将对应电压作为参考电压，然后除以 ADC 的分辨率，以获得电压值，
	// 从上述计算的电压中减去一个偏移量 0.76，计算的分母为比例因子 0.0025，
	// 再加上一个偏移量 25.0，以得到最终的温度值。

	// 在此处进行其他处理操作，例如输出到串口或进行其他计算

	// 输出IN5通道的电压、VREFINT电压和温度到串口

//    printf("IN5 Value: %d V\r\n", value);
//    printf("IN5 Voltage: %.3f V\r\n", (double)voltageIn5);
//    printf("IN6 Value: %d V\r\n", value);
//    printf("IN6 Voltage: %.3f V\r\n", (double)voltageIn6);
//    printf("VREFINT Voltage: %.3f V\r\n", (double)vrefintVoltage);
//    printf("Temperature: %.2f ℃ \r\n", (double)temperature);

}

void HandleADCData(void)
{
    // 平均值变量
    float avgIn5 = 0, avgIn6 = 0, avgTemp = 0;

    // 上次滤波后的值（静态保留）
    static float lastIn5 = 0, lastIn6 = 0, lastTemp = 0;

    // 初始化标志，防止初次误判跳变
    static bool isInited = false;

    // 滤波参数
    const float alpha = 0.2f;               // 滤波系数
    const float jumpRatio = 0.05f;          // 跳变比例阈值：5%

    // 累加器和最值
    uint32_t sumIn5 = 0, sumIn6 = 0, sumTemp = 0;
    uint16_t maxIn5 = 0, minIn5 = 4095;
    uint16_t maxIn6 = 0, minIn6 = 4095;
    uint16_t maxTemp = 0, minTemp = 4095;

    // 遍历DMA缓冲区采样数据（通道顺序：In5, In6, Temp）
    for (uint32_t i = 0; i < ADC_BUFFER_SIZE * 3; i += 3) {
        uint16_t valIn5 = adc_buffer[i];
        uint16_t valIn6 = adc_buffer[i + 1];
        uint16_t valTemp = adc_buffer[i + 2];

        sumIn5 += valIn5;
        sumIn6 += valIn6;
        sumTemp += valTemp;

        if (valIn5 > maxIn5) maxIn5 = valIn5;
        if (valIn5 < minIn5) minIn5 = valIn5;

        if (valIn6 > maxIn6) maxIn6 = valIn6;
        if (valIn6 < minIn6) minIn6 = valIn6;

        if (valTemp > maxTemp) maxTemp = valTemp;
        if (valTemp < minTemp) minTemp = valTemp;
    }

    // 剔除最大/最小值后计算平均
    sumIn5 -= (maxIn5 + minIn5);
    sumIn6 -= (maxIn6 + minIn6);
    sumTemp -= (maxTemp + minTemp);

    uint32_t validCount = ADC_BUFFER_SIZE - 2;
    avgIn5 = sumIn5 / (float)validCount;
    avgIn6 = sumIn6 / (float)validCount;
    avgTemp = sumTemp / (float)validCount;

    // === 初次初始化判断，防止首次跳变误判 ===
    if (!isInited) {
        lastIn5 = avgIn5;
        lastIn6 = avgIn6;
        lastTemp = avgTemp;
        isInited = true;
    }

    // 相对跳变判断（防止误判）
    float deltaIn5 = fabsf(avgIn5 - lastIn5);
    float deltaIn6 = fabsf(avgIn6 - lastIn6);
    float deltaTemp = fabsf(avgTemp - lastTemp);

    bool jumpIn5 = (deltaIn5 / (lastIn5 + 1.0f)) > jumpRatio;
    bool jumpIn6 = (deltaIn6 / (lastIn6 + 1.0f)) > jumpRatio;
    bool jumpTemp = (deltaTemp / (lastTemp + 1.0f)) > jumpRatio;

    // 滤波输出（跳变则直接跟随）
    float filteredIn5 = jumpIn5 ? avgIn5 : (1.0f - alpha) * lastIn5 + alpha * avgIn5;
    float filteredIn6 = jumpIn6 ? avgIn6 : (1.0f - alpha) * lastIn6 + alpha * avgIn6;
    float filteredTemp = jumpTemp ? avgTemp : (1.0f - alpha) * lastTemp + alpha * avgTemp;

    // 保存用于下次
    lastIn5 = filteredIn5;
    lastIn6 = filteredIn6;
    lastTemp = filteredTemp;

    // 后续处理（如电压换算、上下限判断）
    ProcessADCData(filteredIn5, filteredIn6, filteredTemp);
}

//void HandleADCData(void)
//{
//    // 平均值变量
//    float avgIn5 = 0, avgIn6 = 0, avgTemp = 0;
//
//    // 上次滤波后的值（静态变量）
//    static float lastIn5 = 0, lastIn6 = 0, lastTemp = 0;
//
//    // 滤波参数
//    const float alpha = 0.05f;      // 一阶滤波系数（越小越慢）
//    const float jumpRatio = 0.05f;  // 跳变判定比例阈值（5%）
//
//    // 跳变计数器
//    static uint8_t in5JumpCount = 0;
//    static uint8_t in6JumpCount = 0;
//    static uint8_t tempJumpCount = 0;
//
//    // 累加器和最值
//    uint32_t sumIn5 = 0, sumIn6 = 0, sumTemp = 0;
//    uint16_t maxIn5 = 0, minIn5 = 4095;
//    uint16_t maxIn6 = 0, minIn6 = 4095;
//    uint16_t maxTemp = 0, minTemp = 4095;
//
//    // 遍历 DMA 缓冲区采样数据（通道顺序：In5, In6, Temp）
//    for (uint32_t i = 0; i < ADC_BUFFER_SIZE * 3; i += 3) {
//        uint16_t valIn5 = adc_buffer[i];
//        uint16_t valIn6 = adc_buffer[i + 1];
//        uint16_t valTemp = adc_buffer[i + 2];
//
//        sumIn5 += valIn5;
//        sumIn6 += valIn6;
//        sumTemp += valTemp;
//
//        if (valIn5 > maxIn5) maxIn5 = valIn5;
//        if (valIn5 < minIn5) minIn5 = valIn5;
//
//        if (valIn6 > maxIn6) maxIn6 = valIn6;
//        if (valIn6 < minIn6) minIn6 = valIn6;
//
//        if (valTemp > maxTemp) maxTemp = valTemp;
//        if (valTemp < minTemp) minTemp = valTemp;
//    }
//
//    // 剔除最大/最小值后计算平均
//    sumIn5 -= (maxIn5 + minIn5);
//    sumIn6 -= (maxIn6 + minIn6);
//    sumTemp -= (maxTemp + minTemp);
//
//    uint32_t validCount = ADC_BUFFER_SIZE - 2;
//    avgIn5 = sumIn5 / (float)validCount;
//    avgIn6 = sumIn6 / (float)validCount;
//    avgTemp = sumTemp / (float)validCount;
//
//    // 第一次初始化
//    if (lastIn5 == 0) lastIn5 = avgIn5;
//    if (lastIn6 == 0) lastIn6 = avgIn6;
//    if (lastTemp == 0) lastTemp = avgTemp;
//
//    // 跳变判断
//    float deltaIn5 = fabsf(avgIn5 - lastIn5);
//    float deltaIn6 = fabsf(avgIn6 - lastIn6);
//    float deltaTemp = fabsf(avgTemp - lastTemp);
//
//    bool rawJumpIn5 = (deltaIn5 / (lastIn5 + 1.0f)) > jumpRatio;
//    bool rawJumpIn6 = (deltaIn6 / (lastIn6 + 1.0f)) > jumpRatio;
//    bool rawJumpTemp = (deltaTemp / (lastTemp + 1.0f)) > jumpRatio;
//
//    // 多次确认跳变
//    bool jumpIn5 = false, jumpIn6 = false, jumpTemp = false;
//
//    if (rawJumpIn5) {
//        if (++in5JumpCount >= JUMP_CONFIRM_COUNT) {
//            jumpIn5 = true;
//            in5JumpCount = 0;
//        }
//    } else {
//        in5JumpCount = 0;
//    }
//
//    if (rawJumpIn6) {
//        if (++in6JumpCount >= JUMP_CONFIRM_COUNT) {
//            jumpIn6 = true;
//            in6JumpCount = 0;
//        }
//    } else {
//        in6JumpCount = 0;
//    }
//
//    if (rawJumpTemp) {
//        if (++tempJumpCount >= JUMP_CONFIRM_COUNT) {
//            jumpTemp = true;
//            tempJumpCount = 0;
//        }
//    } else {
//        tempJumpCount = 0;
//    }
//
//    // 滤波计算：跳变则直接跟随；否则用一阶低通滤波
//    float filteredIn5   = jumpIn5   ? avgIn5   : (1.0f - alpha) * lastIn5   + alpha * avgIn5;
//    float filteredIn6   = jumpIn6   ? avgIn6   : (1.0f - alpha) * lastIn6   + alpha * avgIn6;
//    float filteredTemp  = jumpTemp  ? avgTemp  : (1.0f - alpha) * lastTemp  + alpha * avgTemp;
//
//    // 保存当前值用于下一次滤波
//    lastIn5  = filteredIn5;
//    lastIn6  = filteredIn6;
//    lastTemp = filteredTemp;
//
//    // 后续处理，如电压换算、状态判断、结果输出
//    ProcessADCData(filteredIn5, filteredIn6, filteredTemp);
//}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) {
        osSemaphoreRelease(adcConversionCompleteSemaphoreHandle);
    }
}


/* USER CODE END 1 */
