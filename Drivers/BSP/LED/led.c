#include "led.h"


//初始化PB1为输出.并使能时钟
//LED IO初始化
void LED_Init(void)
{
	LED0(1);                                                /* 关闭 LED0 */
	LED1(1);                                                /* 关闭 LED1 */
}
