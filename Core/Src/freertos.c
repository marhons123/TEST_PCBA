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
#include "norflash.h"
#include "norflashmanager.h"
#include "sdcardmanager.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// 重定向标准输出流到串口
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
  .priority = (osPriority_t) osPriorityNormal,
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
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask06 */
osThreadId_t myTask06Handle;
const osThreadAttr_t myTask06_attributes = {
  .name = "myTask06",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for vTaskSDCard */
osThreadId_t vTaskSDCardHandle;
const osThreadAttr_t vTaskSDCard_attributes = {
  .name = "vTaskSDCard",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for adcConversionCompleteSemaphore */
osSemaphoreId_t adcConversionCompleteSemaphoreHandle;
const osSemaphoreAttr_t adcConversionCompleteSemaphore_attributes = {
  .name = "adcConversionCompleteSemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void TouchGFX_Task(void *argument);
void adcStartTask(void *argument);
void usart1DataTask(void *argument);
void dacStartTask(void *argument);
void testStartTask(void *argument);
void StartTask06(void *argument);
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

  /* creation of myTask06 */
  myTask06Handle = osThreadNew(StartTask06, NULL, &myTask06_attributes);

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

	//定义全局变量
	uint16_t globalValueA;
	uint16_t globalValueB;

	//在别的地方更新全局变量
	globalValueA = 2000; // 更新全局变量的代码
	globalValueB = 1000; // 更新全局变量的代码

	//启动ADC转换
	//HAL_ADC_Start_DMA(&hadc, (uint32_t*)adc_buffer, ADC_BUFFER_SIZE);
	for (;;) {
		//等待信号量的释放
		osStatus status = osSemaphoreAcquire(
				adcConversionCompleteSemaphoreHandle, osWaitForever);//等待信号量的可用性
		if (status == osOK) {

			// 比较 ADC 采样值和全局变量
			if (value > globalValueA || value < globalValueB) {
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, RESET);
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, SET);
			} else {
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, SET);
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, RESET);
			}
		}
//			// 重新启动 ADC 转换
//			HAL_ADC_Start_DMA(&hadc, (uint32_t*) adc_buffer, ADC_BUFFER_SIZE);
		osDelay(100);
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

//  /* Infinite loop */
	for (;;) {
		// 设置DAC输出值
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 2048是DAC输出的数值

		// 启动DAC输出
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

		// 停止DAC输出
		//HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
		if (dacValue < 4096) {
			dacValue = dacValue + 13;
			dacOut1 = dacValue * 3.3 / 4096;

		} else {
			dacValue = 0;
		}
		osDelay(100);
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
	/* Infinite loop */
	for (;;) {
		// 获取当前显示的屏幕指针
//	         touchgfx::Screen* currentScreen = touchgfx::HAL::getInstance()->getActiveScreen();
//	  touchgfx::Screen* currentScreen = touchgfx::Application::getInstance()->getActiveScreen();

//
//	         // 根据当前屏幕指针执行相应的操作
//	         if (currentScreen == nullptr)
//	         {
//	             // 当前没有显示的屏幕
//	         }
//	         else if (currentScreen == &screen1)
//	         {
//	             // 当前显示的是 screen1
//	         }
//	         else if (currentScreen == &screen2)
//	         {
//	             // 当前显示的是 screen2
//	         }


		// 延时或等待事件
		 vTaskDelay(pdMS_TO_TICKS(100));
//	     }
	}
  /* USER CODE END testStartTask */
}

/* USER CODE BEGIN Header_StartTask06 */
/**
* @brief Function implementing the myTask06 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask06 */
void StartTask06(void *argument)
{
  /* USER CODE BEGIN StartTask06 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);

  }
  /* USER CODE END StartTask06 */
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
	// 初始化 SD 卡和文件系统
	//initSDCard();
	initNorFlash();
    GetDiskInfo();

	for(;;)
	{
		// 周期性保存和加载参数和统计数据
//		loadParametersFromSDCard();
//	    osDelay(10000); // 每10秒加载一次
//
//		saveParametersToSDCard();
//		osDelay(10000); // 每10秒保存一次
//
//		loadStatisticsFromSDCard();
//	    osDelay(10000); // 每10秒加载一次
//
//		saveStatisticsToSDCard();
//		osDelay(10000); // 每10秒保存一次


	}
  /* USER CODE END SDCardManager */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

