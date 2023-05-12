#ifndef __TEST_H__
#define __TEST_H__
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif
	
//#define KEY0        PHin(3) //KEY0按键PH3
//#define KEY1        PHin(2) //KEY1按键PH2
//#define KEY2        PCin(13)//KEY2按键PC13
//#define WK_UP       PAin(0) //WKUP按键PA0
//
//#define IS_KEY0_PRESSED		(KEY0==0)
//#define IS_KEY1_PRESSED		(KEY1==0)
//#define IS_KEY2_PRESSED		(KEY2==0)
//#define IS_WKUP_PRESSED		(WK_UP==1)
//#define IS_KEY_BUSY			(IS_KEY0_PRESSED||IS_KEY1_PRESSED||IS_KEY2_PRESSED||IS_WKUP_PRESSED)
//
//
//typedef enum{
//	KEY_0 = '0',
//	KEY_1,
//	KEY_2,
//	KEY_WKUP,
//	KEY_MAX_NUM = KEY_WKUP-'0'+1,
//	KEY_NONE = 0,
//	KEY_LONG_FLAG = 0x80
//}KEY;//键值
//
//#define KEY_RIGHT		KEY_0
//#define KEY_DOWN		KEY_1
//#define KEY_LEFT		KEY_2
//#define KEY_UP			KEY_WKUP
//
//
////变量申明
//extern uint8_t key_cont_sta[KEY_MAX_NUM];
//extern uint16_t key_long_time[KEY_MAX_NUM];
//extern uint16_t key_cont_interval;
extern uint16_t num;

//
///
uint16_t numadd();
//void KEY_Init(void);
//KEY KEY_Scan(void);
//void KEY_SetContInterval(uint16_t interval);
//void KEY_SetContSta(KEY key,uint8_t sta);
//void KEY_SetLongTime(KEY key,uint16_t time);
//

#ifdef __cplusplus
}
#endif	

#endif
