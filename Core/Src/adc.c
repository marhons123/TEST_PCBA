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

#define VREF 3.3f // 参考电压值
#define ADC_RESOLUTION 4096 // ADC分辨率
#define VREFINT_CAL (*(uint16_t*)0x1FFF75AA) // 内部参考电压校准值

#define AVERAGE_WINDOW_SIZE 20//用于指定计算平均值时使用的窗口大小。例如，如果将AVERAGE_WINDOW_SIZE设置为10，则在计算平均值时，会使用最近的10个采样值进行求平均操作。
#define FILTER_WINDOW_SIZE 5//用于指定滤波器的窗口大小。滤波器窗口大小决定了滤波器对输入数据进行平滑处理的程度。较大的窗口大小会产生更平滑的输出，但也会引入更多的延迟。

// 定义变量
extern osSemaphoreId adcConversionCompleteSemaphoreHandle;//ADC转换完成句柄
uint32_t adc_buffer[ADC_BUFFER_SIZE * 3]; //存储ADC采样数据
float voltageIn5 = 0;         			  // IN5通道电压
float vrefintVoltage = 0;     			  // 内部参考电压
float temperature = 0;        			  // 内部温度
uint16_t value = 0;						  // ADC采样值

// 采样值缓存数组和滤波后的值
uint16_t adcSamplesIn5[AVERAGE_WINDOW_SIZE];					//存储IN5通道的连续采样值。
float filteredValueIn5 = 0;        			//存储经过滤波后的IN5通道数值。

uint16_t adcSamplesVrefint[AVERAGE_WINDOW_SIZE];        	//存储内部参考电压的连续采样值。
float filteredValueVrefint = 0;        			//存储经过滤波后的内部参考电压数值。

uint16_t adcSamplesTemp[AVERAGE_WINDOW_SIZE];        			//存储内部温度的连续采样值。
float filteredValueTemp = 0;        		 //存储经过滤波后的内部温度数值。

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
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
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
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
    HAL_NVIC_SetPriority(ADC_IRQn, 6, 0);
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
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

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
 * @param adcValueIn5 IN5通道的ADC采样值
 * @param adcValueVrefint 内部参考电压的ADC采样值
 * @param adcValueTemp 内部温度的ADC采样值
 */
void ProcessADCData(float averageValueIn5, float averageValueVrefint,
		float averageValueTemp) {

	// 计算IN5通道的电压值
	value = averageValueIn5;
	voltageIn5 = averageValueIn5 * VREF / ADC_RESOLUTION;

	// 计算VREFINT的电压值
	vrefintVoltage = averageValueVrefint * VREF / ADC_RESOLUTION;

	// 计算温度
	temperature = ((averageValueTemp * VREF / ADC_RESOLUTION) - 0.76) / 0.0025
			+ 25.0;
	//首先将平均值乘以参考电压，并除以 ADC 的分辨率，以获得电压值,从上一步计算的电压值中减去一个偏移量 0.76,计算的值除以一个比例因子 0.0025,加上一个偏移量 25.0，以得到最终的温度值。

	// 在此处进行其他处理操作，例如输出到串口或进行其他计算

	// 输出IN5通道的电压、VREFINT电压和温度到串口

//    printf("IN5 Value: %d V\r\n", value);
//    printf("IN5 Voltage: %.3f V\r\n", (double)voltageIn5);
//    printf("VREFINT Voltage: %.3f V\r\n", (double)vrefintVoltage);
//    printf("Temperature: %.2f ℃ \r\n", (double)temperature);

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) {
        osStatus status = osSemaphoreRelease(adcConversionCompleteSemaphoreHandle);
        if (status != osOK) {
            // 处理释放信号量失败的情况
        }

        // 定义用于存储窗口内采样值的数组：
        static uint16_t adcValuesIn5[AVERAGE_WINDOW_SIZE];     // 存储IN5通道的采样值。
        static uint16_t adcValuesVrefint[AVERAGE_WINDOW_SIZE]; // 存储内部参考电压的采样值。
        static uint16_t adcValuesTemp[AVERAGE_WINDOW_SIZE];    // 存储内部温度的采样值。

        // 定义用于计数已处理的采样次数
        static uint32_t samplesProcessed = 0;

        // 循环处理每个采样值：
        for (uint32_t i = 0; i < ADC_BUFFER_SIZE * 3; i += 3) {
            // 获取采样值：
            uint16_t adcValueIn5 = (uint16_t)adc_buffer[i];
            uint16_t adcValueVrefint = (uint16_t)adc_buffer[i + 1];
            uint16_t adcValueTemp = (uint16_t)adc_buffer[i + 2];

            // 将当前采样值存储到对应的数组中：
			adcValuesIn5[(samplesProcessed + 1) % AVERAGE_WINDOW_SIZE] = adcValueIn5;
			adcValuesVrefint[(samplesProcessed + 1) % AVERAGE_WINDOW_SIZE] = adcValueVrefint;
			adcValuesTemp[(samplesProcessed + 1) % AVERAGE_WINDOW_SIZE] = adcValueTemp;

			// 更新已处理的采样值计数：
			samplesProcessed++;

            // 当窗口内采样值达到窗口大小时，执行以下操作：
            if (samplesProcessed >= AVERAGE_WINDOW_SIZE) {
                // 调用 RemoveMinMaxValues 函数，去掉数组中的最大和最小值。
                RemoveMinMaxValues(adcValuesIn5, AVERAGE_WINDOW_SIZE);
                RemoveMinMaxValues(adcValuesVrefint, AVERAGE_WINDOW_SIZE);
                RemoveMinMaxValues(adcValuesTemp, AVERAGE_WINDOW_SIZE);

                // 调用 ComputeAverage 函数，计算去掉最大和最小值后的数组的平均值。
                float averageValueIn5 = ComputeAverage(adcValuesIn5, AVERAGE_WINDOW_SIZE);
                float averageValueVrefint = ComputeAverage(adcValuesVrefint, AVERAGE_WINDOW_SIZE);
                float averageValueTemp = ComputeAverage(adcValuesTemp, AVERAGE_WINDOW_SIZE);

//                // 调用 ProcessADCData 函数，处理平均值后的ADC采样数据。
//                ProcessADCData(averageValueIn5, averageValueVrefint, averageValueTemp);

                //更新滤波后的值，先减去旧的平均值再加上新的平均值。
				filteredValueIn5 -= filteredValueIn5 / FILTER_WINDOW_SIZE;
				filteredValueIn5 += averageValueIn5 / FILTER_WINDOW_SIZE;

				filteredValueVrefint -= filteredValueVrefint / FILTER_WINDOW_SIZE;
				filteredValueVrefint += averageValueVrefint / FILTER_WINDOW_SIZE;

				filteredValueTemp -= filteredValueTemp / FILTER_WINDOW_SIZE;//从 filteredValueIn5 中减去当前的滤波后值的一部分。通过将 filteredValueIn5 除以 FILTER_WINDOW_SIZE，可以得到滤波后值的平均值。这样做是为了平滑滤波后的值，使其更接近新的平均值。
				filteredValueTemp += averageValueTemp / FILTER_WINDOW_SIZE;	//将新的平均值 averageValueIn5 的一部分添加到 filteredValueIn5 中。通过将 averageValueIn5 除以 FILTER_WINDOW_SIZE，可以得到平均值的一部分。这样做是为了逐步更新滤波后的值，以反映新的平均值。
				//这两行代码的作用是在每次回调时更新滤波后的值 filteredValueIn5，以使其逐步接近最新的平均值。

				//调用 ProcessADCData 函数，处理滤波后的ADC采样数据。
				ProcessADCData(filteredValueIn5, filteredValueVrefint,	filteredValueTemp);

				// 清零计数器
                samplesProcessed = 0;
            }
        }
    }
}

/**
 * @brief ADC错误处理回调函数
 * @param hadc ADC句柄
 */
//void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc) {
//	// 在此处进行错误处理操作，例如输出错误信息到串口或进行其他处理
//	printf("ADC Error\r\n");
//}

//该函数用于在给定的数组中去除最小值和最大值。
void RemoveMinMaxValues(uint16_t *array, uint32_t size) {//函数接受一个指向 uint16_t 类型的数组和数组大小作为参数。
	uint16_t minVal = array[0]; //初始化 minVal 变量为数组的第一个元素，作为初始最小值。
	uint16_t maxVal = array[0];	//初始化 maxVal 变量为数组的第一个元素，作为初始最大值。

	// 寻找最小和最大值
	for (uint32_t i = 1; i < size; i++) {//使用循环遍历数组，从索引1开始，逐个比较数组元素与当前的最小值和最大值。
		if (array[i] < minVal) {
			minVal = array[i];	//如果当前元素小于最小值 minVal，则更新 minVal 为当前元素的值。
		}
		if (array[i] > maxVal) {
			maxVal = array[i];	//如果当前元素大于最大值 maxVal，则更新 maxVal 为当前元素的值。
		}
	}

	// 这段代码的作用是在数组中找到最小值和最大值，并将它们替换为数组的第一个元素和最后一个元素。这样做是为了去除最小值和最大值对平均值的影响，从而得到去除极端值的平均值。
	for (uint32_t i = 0; i < size; i++) {
		if (array[i] == minVal) {
			array[i] = array[0];
		}//检查数组中的元素是否等于最小值 minVal。如果相等，则将该元素替换为数组的第一个元素 array[0]。这样就将最小值替换为了数组的第一个元素。
		if (array[i] == maxVal) {
			array[i] = array[size - 1];
		}//检查数组中的元素是否等于最大值 maxVal。如果相等，则将该元素替换为数组的最后一个元素 array[size - 1]。这样就将最大值替换为了数组的最后一个元素。
	}
}

//该函数用于计算给定数组的平均值
float ComputeAverage(uint16_t *array, uint32_t size) {//函数接受一个指向 uint16_t 类型的数组和数组大小作为参数，并返回一个 float 类型的平均值。
	uint32_t sum = 0;	//初始化变量 sum 为0，用于累加数组中的元素值。
	for (uint32_t i = 0; i < size; i++) {//计算总和：使用循环遍历数组，从索引0开始，逐个累加数组中的元素值到变量 sum 中。
		sum += array[i];
	}
	return (float) sum / size;	//计算平均值：将变量 sum 转换为 float 类型，并除以数组大小 size，得到平均值。
}	//使用浮点数除法的目的是得到精确的平均值，而不是整数除法得到的结果。

/* USER CODE END 1 */
