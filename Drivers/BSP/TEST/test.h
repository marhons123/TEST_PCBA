#ifndef TEST_H_
#define TEST_H_

#include <main.h>
#include "stdbool.h"

#define NUM_PARAMETERS 100 // 根据实际情况修改数组大小

// 参数结构体定义
typedef struct {
	uint16_t parameterNumber;                          	// 参数序号
	char productModel[20];                             	// 产品型号
	float ratedVoltage;                                	// 额定电压
	//char contactType[1];                              // 触点类型
	uint16_t samplingTime;			 					// 采样时间
	float inputVoltage1;                      			// 输入电压1
	float inputVoltage2;                              	// 输入电压2
	float inputVoltage3;                    			// 输入电压3
	float startingCoilVoltage;                 			// 启动线圈电压实测值
	float startingCoilVoltageUpperLimit;               	// 启动线圈电压上限
	float startingCoilVoltageLowerLimit;                // 启动线圈电压下限
	uint16_t startingTransitionTime;                	// 启动转换时间实测值
	uint16_t startingTransitionTimeUpperLimit;          // 启动转换时间上限
	uint16_t startingTransitionTimeLowerLimit;          // 启动转换时间下限
	float holdingCoilVoltage;           				// 保持线圈电压实测值
	float holdingCoilVoltageUpperLimit;           		// 保持线圈电压上限
	float holdingCoilVoltageLowerLimit;                 // 保持线圈电压下限
	float holdingVoltage1;                     			// 保持电压1实测值
	float holdingVoltage1UpperLimit;                  	// 保持电压1上限
	float holdingVoltage1LowerLimit;                    // 保持电压1下限
	float holdingVoltage2;                     			// 保持电压2实测值
	float holdingVoltage2UpperLimit;                  	// 保持电压2上限
	float holdingVoltage2LowerLimit;                    // 保持电压2下限
	float holdingVoltage3;                     			// 保持电压3实测值
	float holdingVoltage3UpperLimit;                  	// 保持电压3上限
	float holdingVoltage3LowerLimit;                    // 保持电压3下限
} Parameter;
// 参数数组
extern Parameter parameters[NUM_PARAMETERS];

typedef struct {
	uint16_t totalProduction;  						// 总产量
	uint16_t totalQualified;  						// 总合格品
	uint16_t totalDefective;  						// 总不良品
	uint16_t startingCoilVoltageDefective;  		// 启动线圈电压不良
	uint16_t startingTransitionTimeDefective; 		// 启动线圈时间不良
	uint16_t holdingCoilVoltageDefective;  			// 保持线圈电压不良
	uint16_t holdingVoltage1Defective;  			// 保持电压1不良
	uint16_t holdingVoltage2Defective;  			// 保持电压2不良
	uint16_t holdingVoltage3Defective;  			// 保持电压3不良

} Statistics;
extern Statistics productionCount;

// 定义一个结构体，用于保存一次测试的各项结果状态标志
typedef struct {
    bool startingCoilVoltageNG;     // 启动线圈电压异常标志（NG = Not Good）
    bool startingTransitionNG;  	// 启动过渡时间异常标志
    bool holdingCoilVoltageNG;      // 保持线圈电压异常标志
    bool holdingVoltage1NG;         // 保持电压1异常标志
    bool holdingVoltage2NG;         // 保持电压2异常标志
    bool holdingVoltage3NG;         // 保持电压3异常标志
    bool startingCoiltestFinished;  // 启动线圈测试完成测标志
    bool testFinished;              // 测试完成标志
    bool testPassed;                // 测试通过标志（true 表示测试通过，false 表示失败）
} TestResultFlags;

// 声明一个外部变量 currentResult，用于在其他模块访问当前测试结果标志
extern TestResultFlags currentResult;



extern uint16_t number;
extern uint16_t saveparameterNumber;
extern char MessagesText[50];
extern char messagesText1[50];
extern char messagesText2[50];
extern char messagesText3[50];
extern volatile uint8_t startTestFlag;

void SaveStatisticsToRTC(Statistics *stats);
void LoadStatisticsFromRTC(Statistics *stats);
void InitOrLoadStatistics(Statistics *stats);

#endif /* TEST_H_ */
