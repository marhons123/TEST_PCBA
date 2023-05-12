#include "test.h"


//uint8_t  key_cont_sta[KEY_MAX_NUM] = {0};   //连按功能
//uint16_t key_long_time[KEY_MAX_NUM] = {0};  //长按功能
//uint16_t key_cont_interval = 5;
//
//
uint16_t num;


////按键初始化函数
//void KEY_Init(void)
//{
//	GPIO_InitTypeDef GPIO_Initure;
//
//	__HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
//	__HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟
//	__HAL_RCC_GPIOH_CLK_ENABLE();           //开启GPIOH时钟
//
//	GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
//	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
//	GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
//	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
//	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//
//	GPIO_Initure.Pin=GPIO_PIN_13;           //PC13
//	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
//	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
//	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
//	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
//
//	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PH2,3
//	HAL_GPIO_Init(GPIOH,&GPIO_Initure);
//}
//
////按键处理函数
////返回按键值
//KEY KEY_Scan()
//{
//	static uint8_t is_key_pressed = 0;
//	static uint16_t cont_interval = 0;
//	static uint16_t long_time = 0;
//	static KEY long_pressed_key = KEY_NONE;
//	KEY key = KEY_NONE;
//	uint8_t pos;
//
//
//	if((!is_key_pressed)&&IS_KEY_BUSY)
//	{
//		if(long_time)
//		{
//			long_time--;
//			return KEY_NONE;
//		}
//		if(cont_interval)
//		{
//			cont_interval--;
//			return KEY_NONE;
//		}
//
//		//说明有按键按下了
//		is_key_pressed = 1;
//		if(IS_KEY0_PRESSED)
//			key = KEY_0;
//		else if(IS_KEY1_PRESSED)
//			key = KEY_1;
//		else if(IS_KEY2_PRESSED)
//			key = KEY_2;
//		else if(IS_WKUP_PRESSED)
//			key = KEY_WKUP;
//
//		pos = key-KEY_0;
//		//长按支持
//		if(long_pressed_key!=KEY_NONE)
//		{
//			key = (KEY)(long_pressed_key|KEY_LONG_FLAG);//bit7给置1
//			long_pressed_key = KEY_NONE;
//		}else if((long_time==0)&&key_long_time[pos])
//		{
//			long_time = key_long_time[pos];
//			long_pressed_key = key;
//			is_key_pressed = 0;
//			return KEY_NONE;
//		}
//
//		//连按支持
//		if(key_cont_sta[pos])
//		{
//			cont_interval = key_cont_interval;
//			is_key_pressed = 0;
//		}
//
//	}else if(!IS_KEY_BUSY)
//	{
//		//按键松手了
//		is_key_pressed = 0;
//		cont_interval = 0;
//		if(long_time)
//		{
//			key = long_pressed_key;
//			long_pressed_key = KEY_NONE;
//			long_time = 0;
//		}
//	}
//
//
//	return key;
//}
//
//
////设置连按的时间间隔
//void KEY_SetContInterval(uint16_t interval)
//{
//	key_cont_interval = interval;
//}
////设置长按的时间间隔
//void KEY_SetLongTime(KEY key,uint16_t time)
//{
//	key -= KEY_0;
//	if(key<KEY_MAX_NUM)
//		key_long_time[key] = time;
//}
////设置连按的状态
//void KEY_SetContSta(KEY key,uint8_t sta)
//{
//	key -= KEY_0;
//	if(key<KEY_MAX_NUM)
//		key_cont_sta[key] = sta;
//}
