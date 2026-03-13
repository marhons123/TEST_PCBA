/**
 ****************************************************************************************************
 * @file        atk_rgblcd_touch.h
 * @author
 * @version     V1.0
 * @date        2023-02-24
 * @brief       RGB LCD模块触摸驱动代码
 ****************************************************************************************************
**/

#ifndef __ATK_RGBLCD_TOUCH_H
#define __ATK_RGBLCD_TOUCH_H
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 定义是否使用RGB LCD模块触摸 */
#define ATK_RGBLCD_USING_TOUCH          1

#if (ATK_RGBLCD_USING_TOUCH != 0)

/* 引脚定义 */
#define ATK_RGBLCD_TOUCH_PEN_GPIO_PORT          GPIOH
#define ATK_RGBLCD_TOUCH_PEN_GPIO_PIN           GPIO_PIN_7
#define ATK_RGBLCD_TOUCH_PEN_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)
#define ATK_RGBLCD_TOUCH_TCS_GPIO_PORT          GPIOI
#define ATK_RGBLCD_TOUCH_TCS_GPIO_PIN           GPIO_PIN_8
#define ATK_RGBLCD_TOUCH_TCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)

/* IO操作 */
#define ATK_RGBLCD_TOUCH_READ_PEN()             HAL_GPIO_ReadPin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN)
#define ATK_RGBLCD_TOUCH_PEN(x)                 do{ x ?                                                                                                 \
                                                    HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN, GPIO_PIN_SET) :    \
                                                    HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN, GPIO_PIN_RESET);   \
                                                }while(0)
#define ATK_RGBLCD_TOUCH_TCS(x)                 do{ x ?                                                                                                 \
                                                    HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, ATK_RGBLCD_TOUCH_TCS_GPIO_PIN, GPIO_PIN_SET) :    \
                                                    HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, ATK_RGBLCD_TOUCH_TCS_GPIO_PIN, GPIO_PIN_RESET);   \
                                                }while(0)
/* 错误代码 */
#define ATK_RGBLCD_EOK          0       /* 没有错误 */
#define ATK_RGBLCD_ERROR        1       /* 错误 */
#define ATK_RGBLCD_EINVAL       2       /* 非法参数 */

/* RGB LCD触摸IIC通讯地址枚举 */
typedef enum
{
    ATK_RGBLCD_TOUCH_IIC_ADDR_14 = 0x14,    /* 0x14 */
    ATK_RGBLCD_TOUCH_IIC_ADDR_5D = 0x5D,    /* 0x5D */
    ATK_RGBLCD_TOUCH_IIC_ADDR_38 = 0x38,    /* 0x38 */
} atk_rgblcd_touch_iic_addr_t;

/* RGB LCD触摸IC类型枚举 */
typedef enum
{
    ATK_RGBLCD_TOUCH_TYPE_GTXX = 0x00,  /* GT9111、GT9147、GT9271 */
    ATK_RGBLCD_TOUCH_TYPE_FTXX,         /* FT5206 */
} atk_rgblcd_touch_type_t;

/* RGB LCD模块PID枚举 */
enum {
    ATK_RGBLCD_PID_4342 = 0x4342,       /* ATK-MD0430R-480272 */
    ATK_RGBLCD_PID_7084 = 0x7084,       /* ATK-MD0700R-800480 */
    ATK_RGBLCD_PID_7016 = 0x7016,       /* ATK-MD0700R-1024600 */
    ATK_RGBLCD_PID_7018 = 0x7018,       /* ATK-MD0700R-1280800 */
    ATK_RGBLCD_PID_4384 = 0x4384,       /* ATK-MD0430R-800480 */
    ATK_RGBLCD_PID_1018 = 0x1018,       /* ATK-MD1010R-1280800 */
};



/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;     /* 触摸点X坐标 */
    uint16_t y;     /* 触摸点Y坐标 */
    uint16_t size;  /* 触摸点大小 */
} atk_rgblcd_touch_point_t;

/* 错误代码 */
#define ATK_RGBLCD_TOUCH_EOK                    0   /* 没有错误 */
#define ATK_RGBLCD_TOUCH_ERROR                  1   /* 错误 */

/* 操作函数 */
uint8_t atk_rgblcd_touch_init(atk_rgblcd_touch_type_t type);                    /* RGB LCD模块触摸初始化 */
uint8_t atk_rgblcd_touch_scan(atk_rgblcd_touch_point_t *point, uint8_t cnt);    /* RGB LCD模块触摸扫描 */

#endif

#ifdef __cplusplus
}
#endif
#endif
