/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.h
 * @brief   This file contains all the function prototypes for
 *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */

#define ADC_BUFFER_SIZE 1//ADC缓冲区的大小


extern uint32_t adc_buffer[ADC_BUFFER_SIZE * 3];//存储ADC采样数据
extern float voltageIn5;         // IN5通道电压
extern float vrefintVoltage;     // 内部参考电压
extern float temperature;        // 内部温度
extern uint16_t value;			 //ADC采样值

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void ProcessADCData(float averageValueIn5, float averageValueVrefint, float averageValueTemp);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void RemoveMinMaxValues(uint16_t *array, uint32_t size);
float ComputeAverage(uint16_t *array, uint32_t size);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

