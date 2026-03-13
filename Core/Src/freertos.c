/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "test.h"
#include "key.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
#include "stdio.h"
#include "stdbool.h"
#include "norflash.h"
#include "norflashmanager.h"
#include "soft_i2c.h"
#include "at24c02.h"
#include "ads1115.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern SoftI2C_HandleTypeDef hi2c_eeprom;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

extern UART_HandleTypeDef huart1;

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	return ch;
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for touchGFX */
osThreadId_t touchGFXHandle;
const osThreadAttr_t touchGFX_attributes = {
  .name = "touchGFX",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for adcTask */
osThreadId_t adcTaskHandle;
const osThreadAttr_t adcTask_attributes = {
  .name = "adcTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for usart1Data */
osThreadId_t usart1DataHandle;
const osThreadAttr_t usart1Data_attributes = {
  .name = "usart1Data",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for dacTask */
osThreadId_t dacTaskHandle;
const osThreadAttr_t dacTask_attributes = {
  .name = "dacTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for testTask */
osThreadId_t testTaskHandle;
const osThreadAttr_t testTask_attributes = {
  .name = "testTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for vTaskSDCard */
osThreadId_t vTaskSDCardHandle;
const osThreadAttr_t vTaskSDCard_attributes = {
  .name = "vTaskSDCard",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for adcConversionCompleteSemaphore */
osSemaphoreId_t adcConversionCompleteSemaphoreHandle;
const osSemaphoreAttr_t adcConversionCompleteSemaphore_attributes = {
  .name = "adcConversionCompleteSemaphore"
};
/* Definitions for testStartSemaphore */
osSemaphoreId_t testStartSemaphoreHandle;
const osSemaphoreAttr_t testStartSemaphore_attributes = {
  .name = "testStartSemaphore"
};
/* Definitions for clearDisplaySemaphore */
osSemaphoreId_t clearDisplaySemaphoreHandle;
const osSemaphoreAttr_t clearDisplaySemaphore_attributes = {
  .name = "clearDisplaySemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void TouchGFX_Task(void *argument);
void adcStartTask(void *argument);
void usart1DataTask(void *argument);
void dacStartTask(void *argument);
void testStartTask(void *argument);
void SDCardManager(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of adcConversionCompleteSemaphore */
  adcConversionCompleteSemaphoreHandle = osSemaphoreNew(1, 0, &adcConversionCompleteSemaphore_attributes);

  /* creation of testStartSemaphore */
  testStartSemaphoreHandle = osSemaphoreNew(1, 0, &testStartSemaphore_attributes);

  /* creation of clearDisplaySemaphore */
  clearDisplaySemaphoreHandle = osSemaphoreNew(1, 0, &clearDisplaySemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of touchGFX */
  touchGFXHandle = osThreadNew(TouchGFX_Task, NULL, &touchGFX_attributes);

  /* creation of adcTask */
  adcTaskHandle = osThreadNew(adcStartTask, NULL, &adcTask_attributes);

  /* creation of usart1Data */
  usart1DataHandle = osThreadNew(usart1DataTask, NULL, &usart1Data_attributes);

  /* creation of dacTask */
  dacTaskHandle = osThreadNew(dacStartTask, NULL, &dacTask_attributes);

  /* creation of testTask */
  testTaskHandle = osThreadNew(testStartTask, NULL, &testTask_attributes);

  /* creation of vTaskSDCard */
  vTaskSDCardHandle = osThreadNew(SDCardManager, NULL, &vTaskSDCard_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_TouchGFX_Task */
/**
 * @brief  Function implementing the touchGFX thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_TouchGFX_Task */
__weak void TouchGFX_Task(void *argument)
{
  /* USER CODE BEGIN TouchGFX_Task */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END TouchGFX_Task */
}

/* USER CODE BEGIN Header_adcStartTask */
/**
 * @brief Function implementing the adcTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_adcStartTask */
void adcStartTask(void *argument)
{
  /* USER CODE BEGIN adcStartTask */
	/* Infinite loop */

	HAL_ADC_Start_DMA(&hadc1, adc_buffer, ADC_BUFFER_SIZE * 3);

	for (;;) {
//	    	HandleADCData();
//
//	        // 等待DMA完成信号
//	        if (osSemaphoreAcquire(adcConversionCompleteSemaphoreHandle, osWaitForever) == osOK) {
//
//	        	HandleADCData();
//	        }

		osDelay(10);
	}

  /* USER CODE END adcStartTask */
}

/* USER CODE BEGIN Header_usart1DataTask */
/**
 * @brief Function implementing the usart1Data thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_usart1DataTask */
void usart1DataTask(void *argument)
{
  /* USER CODE BEGIN usart1DataTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END usart1DataTask */
}

/* USER CODE BEGIN Header_dacStartTask */
/**
 * @brief Function implementing the dacTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_dacStartTask */
void dacStartTask(void *argument)
{
  /* USER CODE BEGIN dacStartTask */
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

//  /* Infinite loop */
	for (;;) {

//	    osSemaphoreAcquire(testStartSemaphoreHandle, osWaitForever);  // 等待信号量
//
//		 dacValue = voltageToDAC(parameters[number].ratedVoltage);
//		 dacOut1 = parameters[number].ratedVoltage;
//		 HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 2048  DAC       ֵ
//		 HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//		 osDelay(500);
//		 HandleADCData();
//
//		 dacValue = voltageToDAC(parameters[number].inputVoltage1);
//		 dacOut1 = parameters[number].inputVoltage1;
//		 HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 2048  DAC       ֵ
//		 HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//		 osDelay(500);
//		 HandleADCData();
//
//		 dacValue = voltageToDAC(parameters[number].inputVoltage2);
//		 dacOut1 = parameters[number].inputVoltage2;
//		 HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 2048  DAC       ֵ
//		 HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//		 osDelay(500);
//		 HandleADCData();
//
//		 dacValue = voltageToDAC(parameters[number].inputVoltage3);
//		 dacOut1 = parameters[number].inputVoltage3;
//		 HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 2048  DAC       ֵ
//		 HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//		 osDelay(500);
//		 HandleADCData();
//
//
//
//		 // 以此类推……

		osDelay(1);
	}
  /* USER CODE END dacStartTask */
}

/* USER CODE BEGIN Header_testStartTask */
/**
 * @brief Function implementing the testTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_testStartTask */
void testStartTask(void *argument)
{
  /* USER CODE BEGIN testStartTask */
	InitOrLoadStatistics(&productionCount);

	/* Infinite loop */
	for (;;) {
		// 等待UI或按键触发测试开始
		if (osSemaphoreAcquire(testStartSemaphoreHandle, osWaitForever)
				== osOK) {
			bool ok = true;  								// 用于记录测试是否通过，默认为通过

			// ------- 在每次测试前重置不良标志 -------
			currentResult.testFinished = false; 			// 重置测试完成标志
			currentResult.testPassed = false; 				// 重置测试通过标志
			currentResult.startingCoilVoltageNG = false;  	// 重置启动线圈电压不良标志
			currentResult.holdingCoilVoltageNG = false;     // 重置保持线圈电压不良标志
			currentResult.startingCoiltestFinished = false; // 重置测试完成标志
			currentResult.holdingVoltage1NG = false;     	// 重置保持电压1不良标志
			currentResult.holdingVoltage2NG = false;     	// 重置保持电压2不良标志
			currentResult.holdingVoltage3NG = false;     	// 重置保持电压3不良标志
			//currentResult.startingTransitionTimeNG = false; // 重置启动转换时间不良标志
			currentResult.startingTransitionNG = false; 	// 重置启动转换不良标志

			// ------- 清除前一次测试的IO状态和灯状态 -------
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); // 关闭电压输出指示灯输出灯
			HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_SET); // 关闭电压输出
			HAL_GPIO_WritePin(OK_GPIO_Port, OK_Pin, GPIO_PIN_SET);     // 关闭OK输出
			HAL_GPIO_WritePin(NG_GPIO_Port, NG_Pin, GPIO_PIN_SET);     // 关闭NG输出

			HAL_GPIO_WritePin(startingCoilVoltageNG_GPIO_Port,
			startingCoilVoltageNG_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(holdingCoilVoltageNG_GPIO_Port,
			holdingCoilVoltageNG_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(startingTransitionTimeNG_GPIO_Port,
			startingTransitionTimeNG_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(holdingVoltage1NG_GPIO_Port,
			holdingVoltage1NG_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(holdingVoltage2NG_GPIO_Port,
			holdingVoltage2NG_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(holdingVoltage3NG_GPIO_Port,
			holdingVoltage3NG_Pin, GPIO_PIN_SET);

//			// ------- 第一步：输出设定电压到DAC，检测两路电压是否合格 -------
//			dacValue = voltageToDAC(parameters[number].ratedVoltage);				//输出额定电压
//			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); 		// 设置DAC输出
//			osDelay(200);  // 等待电压稳定
//			HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_RESET); 	// 打开电压输出
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET); 			// 打开输出灯
//			uint32_t startTick = osKernelGetTickCount();
//			uint32_t detectedTick = 0;
//			bool fallingEdgeDetected = false;
//
//			// 进入检测循环，直到超时或检测到下降沿
//			while ((osKernelGetTickCount() - startTick) < parameters[number].startingTransitionTimeUpperLimit) {
//			    // 采样
//				HandleADCData();
//			    parameters[number].startingCoilVoltage = voltageIn5 * 20;  // *20 分压还原
//			    parameters[number].holdingCoilVoltage = voltageIn6 * 20;
//
//			    // 检查启动线圈电压是否在合格范围
//			    if (parameters[number].startingCoilVoltage < parameters[number].startingCoilVoltageLowerLimit ||
//			        parameters[number].startingCoilVoltage > parameters[number].startingCoilVoltageUpperLimit) {
//			        currentResult.startingCoilVoltageNG = true;
//			        HAL_GPIO_WritePin(startingCoilVoltageNG_GPIO_Port, startingCoilVoltageNG_Pin, GPIO_PIN_RESET);
//			        ok = false;
//			    }
//
//			    // 检查保持电压是否在合格范围
//			    if (parameters[number].holdingCoilVoltage < parameters[number].holdingCoilVoltageLowerLimit ||
//			        parameters[number].holdingCoilVoltage > parameters[number].holdingCoilVoltageUpperLimit) {
//			        currentResult.holdingCoilVoltageNG = true;
//			        HAL_GPIO_WritePin(holdingCoilVoltageNG_GPIO_Port, holdingCoilVoltageNG_Pin, GPIO_PIN_RESET);
//			        ok = false;
//			    }
//
//			    // 检测 startingCoilVoltage 是否首次低于下限，记录时间
//			    if (!fallingEdgeDetected &&
//			        parameters[number].startingCoilVoltage <= parameters[number].startingCoilVoltageLowerLimit) {
//			        detectedTick = osKernelGetTickCount();
//			        fallingEdgeDetected = true;
//			    }
//
//			    if (fallingEdgeDetected) break; // 检测到后退出
//			    osDelay(5);
//			}
//
//			// 检查结果，记录时间
//			if (!fallingEdgeDetected) {
//			    // 超时未检测到下降沿
//			    currentResult.startingTransitionTimeNG = true;
//			    HAL_GPIO_WritePin(startingTransitionTimeNG_GPIO_Port, startingTransitionTimeNG_Pin, GPIO_PIN_RESET);
//			    ok = false;
//			    parameters[number].startingTransitionTime = parameters[number].startingTransitionTimeUpperLimit;
//			} else {
//			    uint32_t delta = detectedTick - startTick;
//			    parameters[number].startingTransitionTime = delta;
//
//			    if (delta < parameters[number].startingTransitionTimeLowerLimit ||
//			        delta > parameters[number].startingTransitionTimeUpperLimit) {
//			        currentResult.startingTransitionTimeNG = true;
//			        HAL_GPIO_WritePin(startingTransitionTimeNG_GPIO_Port, startingTransitionTimeNG_Pin, GPIO_PIN_RESET);
//			        ok = false;
//			    }
//			}
			osDelay(10);
			// ------- 第一步：输出设定电压到DAC，检测两路电压是否合格 -------
			dacValue = voltageToDAC(parameters[number].ratedVoltage);	//输出额定电压
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 设置DAC输出
			osDelay(200);  // 等待电压稳定
			HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_RESET); // 打开电压输出
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET); // 打开输出灯

			osDelay(parameters[number].startingTransitionTimeLowerLimit);
			if (osSemaphoreAcquire(adcConversionCompleteSemaphoreHandle, 100)
					== osOK) {
				HandleADCData();
				parameters[number].startingCoilVoltage = voltageIn5 * 20.4;
				if (parameters[number].startingCoilVoltage
						< parameters[number].startingCoilVoltageLowerLimit
						|| parameters[number].startingCoilVoltage
								> parameters[number].startingCoilVoltageUpperLimit) {
					currentResult.startingCoilVoltageNG = true;
					HAL_GPIO_WritePin(startingCoilVoltageNG_GPIO_Port,
					startingCoilVoltageNG_Pin, GPIO_PIN_RESET);
					ok = false;
				}

			} else {
				ok = false;
				currentResult.startingCoilVoltageNG = true;  // 设置不良标志
			}

			if (osSemaphoreAcquire(adcConversionCompleteSemaphoreHandle, 100)
					== osOK) {
				HandleADCData();
				parameters[number].holdingCoilVoltage = voltageIn6 * 20.2;
				// 检查保持电压是否在合格范围
				if (parameters[number].holdingCoilVoltage
						< parameters[number].holdingCoilVoltageLowerLimit
						|| parameters[number].holdingCoilVoltage
								> parameters[number].holdingCoilVoltageUpperLimit) {
					currentResult.holdingCoilVoltageNG = true;
					HAL_GPIO_WritePin(holdingCoilVoltageNG_GPIO_Port,
					holdingCoilVoltageNG_Pin, GPIO_PIN_RESET);
					ok = false;
				}

			} else {
				ok = false;
				currentResult.holdingCoilVoltageNG = true;
			}


			osDelay(
					parameters[number].startingTransitionTimeUpperLimit
							- parameters[number].startingTransitionTimeLowerLimit);
			if (osSemaphoreAcquire(adcConversionCompleteSemaphoreHandle, 100)
					== osOK) {
				HandleADCData();
				float startingCoilOffVoltage = voltageIn5 * 20.3;
				if (startingCoilOffVoltage > 1) {
					currentResult.startingTransitionNG = true;
					HAL_GPIO_WritePin(startingTransitionTimeNG_GPIO_Port,
							startingTransitionTimeNG_Pin, GPIO_PIN_RESET);
					ok = false;
				}

			} else {
				ok = false;
				currentResult.startingCoilVoltageNG = true;  // 设置不良标志
			}

			currentResult.startingCoiltestFinished = true;

			// ------- 第二步：输出输入电压1，检测保持电压1（PA6） -------
			dacValue = voltageToDAC(parameters[number].inputVoltage1);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 设置DAC输出电压
			osDelay(300);  // 等待电压稳定
			//HAL_GPIO_WritePin(K_ON_GPIO_Port, K_ON_Pin, GPIO_PIN_SET); // 分压由20:1切换到2:1

			if (osSemaphoreAcquire(adcConversionCompleteSemaphoreHandle, 100)
					== osOK) {
				HandleADCData();
				parameters[number].holdingVoltage1 = voltageIn6 * 20.15;

				if (parameters[number].holdingVoltage1
						< parameters[number].holdingVoltage1LowerLimit
						|| parameters[number].holdingVoltage1
								> parameters[number].holdingVoltage1UpperLimit) {
					ok = false;
					currentResult.holdingVoltage1NG = true;  // 设置不良标志
					HAL_GPIO_WritePin(holdingVoltage1NG_GPIO_Port,
					holdingVoltage1NG_Pin, GPIO_PIN_RESET);
				}
			} else {
				ok = false;
				currentResult.holdingVoltage1NG = true;  // 设置不良标志
			}

//			// ------- 第三步：重复输入电压2，检测保持电压2（PA6） -------
//			dacValue = voltageToDAC(parameters[number].inputVoltage2);
//			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 设置DAC输出电压
//			osDelay(300);  // 等待电压稳定
//
//			if (osSemaphoreAcquire(adcConversionCompleteSemaphoreHandle, 100)
//					== osOK) {
//				HandleADCData();
//				parameters[number].holdingVoltage2 = voltageIn6 * 20.15;
//
//				if (parameters[number].holdingVoltage2
//						< parameters[number].holdingVoltage2LowerLimit
//						|| parameters[number].holdingVoltage2
//								> parameters[number].holdingVoltage2UpperLimit) {
//					ok = false;
//					currentResult.holdingVoltage2NG = true;  // 设置不良标志
//					HAL_GPIO_WritePin(holdingVoltage2NG_GPIO_Port,
//					holdingVoltage2NG_Pin, GPIO_PIN_RESET);
//				}
//			} else {
//				ok = false;
//				currentResult.holdingVoltage2NG = true;  // 设置不良标志
//			}

			// ------- 第四步：输出输入电压3，检测保持电压3（PA6） -------
			dacValue = voltageToDAC(parameters[number].inputVoltage3);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 设置DAC输出电压
			osDelay(300);  // 等待电压稳定

			if (osSemaphoreAcquire(adcConversionCompleteSemaphoreHandle, 100)
					== osOK) {
				HandleADCData();
				parameters[number].holdingVoltage3 = voltageIn6 * 20.15;

				if (parameters[number].holdingVoltage3
						< parameters[number].holdingVoltage3LowerLimit
						|| parameters[number].holdingVoltage3
								> parameters[number].holdingVoltage3UpperLimit) {
					ok = false;
					currentResult.holdingVoltage3NG = true;  // 设置不良标志
					HAL_GPIO_WritePin(holdingVoltage3NG_GPIO_Port,
					holdingVoltage3NG_Pin, GPIO_PIN_RESET);
				}
			} else {
				ok = false;
				currentResult.holdingVoltage3NG = true;  // 设置不良标志
			}

			// ------- 测试结果处理 -------
			if (ok) {
				productionCount.totalQualified++;  // 合格数量增加
				currentResult.testPassed = true;
				HAL_GPIO_WritePin(OK_GPIO_Port, OK_Pin, GPIO_PIN_RESET); // OK灯亮
			} else {
				productionCount.totalDefective++;  // 不合格数量增加
				currentResult.testPassed = false;
				HAL_GPIO_WritePin(NG_GPIO_Port, NG_Pin, GPIO_PIN_RESET); // NG灯亮
			}

			productionCount.totalProduction++;    // 总测试次数 +1
			currentResult.testFinished = true;
			SaveStatisticsToRTC(&productionCount);
			osDelay(200);
			HAL_GPIO_WritePin(NG_GPIO_Port, NG_Pin, GPIO_PIN_SET); // NG灯灭
		}

		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); // 关闭电压输出指示灯输出灯
		HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_SET); // 关闭电压输出
		osDelay(100);

	}
  /* USER CODE END testStartTask */
}

/* USER CODE BEGIN Header_SDCardManager */
/**
 * @brief Function implementing the vTaskSDCard thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_SDCardManager */
void SDCardManager(void *argument)
{
  /* USER CODE BEGIN SDCardManager */
	/* Infinite loop */
	//initSDCard();
	initNorFlash();
	GetDiskInfo();
//	AT24C02_Test();

	for (;;) {

//		loadParametersFromSDCard();
//	    osDelay(10000); //
//
//		saveParametersToSDCard();
//		osDelay(10000); //
//
//		loadStatisticsFromSDCard();
//	    osDelay(10000); //
//
//		saveStatisticsToSDCard();
//		osDelay(10000); //
//		ADS1115_Test();//i2c 测量电压模块

	}
	osDelay(3000);  // 周期检查每100ms
  /* USER CODE END SDCardManager */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

