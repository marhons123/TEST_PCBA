/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.21.3. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
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

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include "ft5206.h"

void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */
	   FT5206_Init();//触摸初始化
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */
	u8 buf[4];
	if (FT_INT == 0) //有手指按下时,会一直输出低电平
			{
		FT5206_RD_Reg(FT_REG_NUM_FINGER, buf, 1); //读取触摸点的状态
		if (buf[0] & 0x0f) //是真的有手指按下
				{
			FT5206_RD_Reg(FT_TP1_REG, buf, 4); //读取第1点的坐标值
			y = ((u16) (buf[0] & 0X0F) << 8) + buf[1];
			x = ((u16) (buf[2] & 0X0F) << 8) + buf[3];
			return true;
		}
	}
	return false;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
