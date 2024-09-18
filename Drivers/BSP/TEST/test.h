#ifndef TEST_H_
#define TEST_H_

#include <main.h>

#define NUM_PARAMETERS 100 // 根据实际情况修改数组大小

// 参数结构体定义
typedef struct {
	uint16_t parameterNumber;                          // 参数序号
	char productModel[20];                             // 产品型号,放在TestVariables
	float ratedVoltage;                                // 额定电压
	char contactType[1];                               // 触点类型,放在TestVariables
	float ambientTemperatureMeasuredValue;			   // 环境温度实测值
	float temperatureCoefficient;                      // 温度系数
	float contactCurrent;                              // 触点电流
	float coilResistanceUpperLimit;                    // 线圈电阻上限
	float coilResistanceMeasuredValue;                 // 线圈电阻实测值
	float coilResistanceConversionValue;               // 线圈电阻转换值
	float coilResistanceLowerLimit;                    // 线圈电阻下限
	float contactResistanceUpperLimit;                 // 接触电阻上限
	float contactResistanceMeasuredValueNO1;           // 接触电阻实测值NO1
	float contactResistanceMeasuredValueNC1;           // 接触电阻实测值NC1
	float contactResistanceMeasuredValueNO2;           // 接触电阻实测值NO2
	float contactResistanceMeasuredValueNC2;           // 接触电阻实测值NC2
	float contactResistanceLowerLimit;                 // 接触电阻下限
	float pullInVoltageUpperLimit;                     // 吸合电压上限
	float pullInVoltageMeasuredValue;                  // 吸合电压实测值
	float pullInVoltageLowerLimit;                     // 吸合电压下限
	float secondaryPullInVoltage;                      // 二次吸合电压
	float releaseVoltageUpperLimit;                    // 释放电压上限
	float releaseVoltageMeasuredValue;                 // 释放电压实测值
	float releaseVoltageLowerLimit;                    // 释放电压下限
	float pullInTimeUpperLimit;                        // 吸合时间上限
	float pullInTimeMeasuredValue;                     // 吸合时间实测值
	float pullInTimeLowerLimit;                        // 吸合时间下限
	float pullInBounceTimeUpperLimit;                  // 吸合回跳时间上限
	float pullInBounceTimeMeasuredValue;               // 吸合回跳时间实测值
	float pullInBounceTimeLowerLimit;                  // 吸合回跳时间下限
	float releaseTimeUpperLimit;                       // 释放时间上限
	float releaseTimeMeasuredValue;                    // 释放时间实测值
	float releaseTimeLowerLimit;                       // 释放时间下限
	float releaseBounceTimeUpperLimit;                 // 释放回跳时间上限
	float releaseBounceTimeMeasuredValue;              // 释放回跳时间实测值
	float releaseBounceTimeLowerLimit;                 // 释放回跳时间下限
	float syncTimeUpperLimit;                          // 同步时间上限
	float syncTimeMeasuredValue;                       // 同步时间实测值
	float syncTimeLowerLimit;                          // 同步时间下限
} Parameter;
// 参数数组
extern Parameter parameters[NUM_PARAMETERS];

typedef struct {
	int totalProduction;  // 总产量
	int totalQualified;  // 总合格品
	int totalDefective;  // 总不良品
	int coilDefective;  // 线圈不良
	int contactResistanceDefective;  // 接触电阻不良
	int pullInVoltageDefective;  // 吸合电压不良
	int secondaryPullInDefective;  // 二次吸合不良
	int releaseVoltageDefective;  // 释放电压不良
	int pullInTimeDefective;  // 吸合时间不良
	int pullInBounceDefective;  // 吸合回跳不良
	int releaseTimeDefective;  // 释放时间不良
	int releaseBounceDefective;  // 释放回跳不良
	int syncTimeDefective;  // 同步时间不良
} Statistics;

extern Statistics productionCount;
extern uint16_t number;
extern char MessagesText[50];
extern char messagesText1[50];
extern char messagesText2[50];
extern char messagesText3[50];

#endif /* TEST_H_ */
