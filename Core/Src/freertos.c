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
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId touchGFXHandle;
osThreadId ledTaskHandle;
osThreadId myTask03Handle;
osThreadId adcTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void TouchGFX_Task(void const * argument);
void StartLEDTask(void const * argument);
void StartTask03(void const * argument);
void adcStartTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
  /* definition and creation of touchGFX */
  osThreadDef(touchGFX, TouchGFX_Task, osPriorityNormal, 0, 2048);
  touchGFXHandle = osThreadCreate(osThread(touchGFX), NULL);

  /* definition and creation of ledTask */
  osThreadDef(ledTask, StartLEDTask, osPriorityIdle, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityBelowNormal, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of adcTask */
  osThreadDef(adcTask, adcStartTask, osPriorityNormal, 0, 1024);
  adcTaskHandle = osThreadCreate(osThread(adcTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_TouchGFX_Task */
/**
 * @brief  Function implementing the touchGFX thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_TouchGFX_Task */
__weak void TouchGFX_Task(void const * argument)
{
  /* USER CODE BEGIN TouchGFX_Task */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END TouchGFX_Task */
}

/* USER CODE BEGIN Header_StartLEDTask */
/**
 * @brief Function implementing the LEDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLEDTask */
void StartLEDTask(void const * argument)
{
  /* USER CODE BEGIN StartLEDTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1000);
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	}
  /* USER CODE END StartLEDTask */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the myTask03 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
	/* Infinite loop */
	for (;;) {
		osDelay(1000);
		if (num < 100) {
			num++;
		} else
			num = 0;
	}
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_adcStartTask */
/**
 * @brief Function implementing the adcTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_adcStartTask */
void adcStartTask(void const * argument)
{
  /* USER CODE BEGIN adcStartTask */
	/* Infinite loop */
	for (;;) {

//		HAL_ADC_Start(&hadc1);//Æô¶¯DAC
//		if (HAL_ADC_PollForConversion(&hadc1, 20) == HAL_OK) {
//			val = HAL_ADC_GetValue(&hadc1);
//			volt = 3.3f*val/4096;
//		}

		osDelay(500);
	}
  /* USER CODE END adcStartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
