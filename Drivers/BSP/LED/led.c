#include "led.h"


//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void LED_Init(void)
{
	

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0置1 
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);	//PB1置1  
}
