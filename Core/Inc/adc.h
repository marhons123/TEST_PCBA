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

#define ADC_BUFFER_SIZE 300//姣忎釜閫氶亾閲囨牱xi
extern uint32_t adc_buffer[ADC_BUFFER_SIZE * 3];// 瀛樺偍ADC閲囨牱鏁版嵁
extern float voltageIn5;         //IN5閫氶亾鐢靛帇
extern float voltageIn6;         //IN6閫氶亾鐢靛帇
extern float temperature;        // 鍐呴儴娓╁害
extern uint16_t value1;			 //ADC閲囨牱鍊�
extern uint16_t value2;			 //ADC閲囨牱鍊贾�

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void ProcessADCData(float averageValueIn5, float averageValueVrefint, float averageValueTemp);
void HandleADCData(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void RemoveMinMaxValues(uint16_t *array, uint32_t size);
float ComputeAverage(uint16_t *array, uint32_t size);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

