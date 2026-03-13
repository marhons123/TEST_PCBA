#include "test.h"
#include <string.h>
#include "rtc.h"

Parameter parameters[NUM_PARAMETERS];
Statistics productionCount;
TestResultFlags currentResult = { 0 };  // 所有默认是 OK
uint16_t number;
uint16_t saveparameterNumber;
char MessagesText[50] = "";
char messagesText1[50] = "1234";
char messagesText2[50] = "abcd";
char messagesText3[50] = "ABCD";
volatile uint8_t startTestFlag = 0;



#define RTC_MAGIC_FLAG 0xA5A5

void SaveStatisticsToRTC(Statistics *stats)
{
    __HAL_RCC_PWR_CLK_ENABLE();          // 1. 打开PWR时钟
    HAL_PWR_EnableBkUpAccess();          // 2. 解锁备份域访问

    uint32_t *data = (uint32_t *)stats;
    for (int i = 0; i < sizeof(Statistics) / 4; i++)
    {
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0 + i, data[i]);
    }
}

void LoadStatisticsFromRTC(Statistics *stats)
{
    __HAL_RCC_PWR_CLK_ENABLE();          // 1. 打开PWR时钟
    HAL_PWR_EnableBkUpAccess();          // 2. 解锁备份域访问

    uint32_t *data = (uint32_t *)stats;
    for (int i = 0; i < sizeof(Statistics) / 4; i++)
    {
        data[i] = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0 + i);
    }
}

#define RTC_MAGIC_FLAG 0xA5A5

void InitOrLoadStatistics(Statistics *stats)
{
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR13) != RTC_MAGIC_FLAG)
    {
        memset(stats, 0, sizeof(Statistics));
        SaveStatisticsToRTC(stats);
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR13, RTC_MAGIC_FLAG);
    }
    else
    {
        LoadStatisticsFromRTC(stats);
    }
}

