#ifndef SETTINGVIEW_HPP
#define SETTINGVIEW_HPP

#include <gui_generated/setting_screen/SettingViewBase.hpp>
#include <gui/setting_screen/SettingPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>
#include "gui/common/TestVariables.hpp"
#include "test.h"

#include "norflashmanager.h"
#include <string.h>
#include <stdio.h> // 添加用于串口打印的库

// 定义电压最大值常量
#define MAX_VOLTAGE_LIMIT 60.0f

class SettingView: public SettingViewBase {
public:
	SettingView();
	virtual ~SettingView() {
	}
	virtual void setupScreen();
	virtual void tearDownScreen();

	//下面为屏幕点击相关
	virtual void resetCounterClicked();
	virtual void okClicked();
	virtual void exitClicked();
	virtual void parameterNumberClicked();
	virtual void productModelClicked();
	virtual void ratedVoltageClicked();
	virtual void samplingTimeClicked();
	virtual void inputVoltage1Clicked();
	virtual void inputVoltage2Clicked();
	virtual void inputVoltage3Clicked();
	virtual void startingCoilVoltageUpperLimitClicked();
	virtual void startingCoilVoltageLowerLimitClicked();
	virtual void startingTransitionTimeUpperLimitClicked();
	virtual void startingTransitionTimeLowerLimitClicked();
	virtual void holdingCoilVoltageUpperLimitClicked();
	virtual void holdingCoilVoltageLowerLimitClicked();
	virtual void holdingVoltage1UpperLimitClicked();
	virtual void holdingVoltage1LowerLimitClicked();
	virtual void holdingVoltage2UpperLimitClicked();
	virtual void holdingVoltage2LowerLimitClicked();
	virtual void holdingVoltage3UpperLimitClicked();
	virtual void holdingVoltage3LowerLimitClicked();
	virtual void keyBoardShow();
	virtual void keyBoardHide();

	void loadParameters();           // 从结构体加载数据到UI
	void initializeParameters();     // 初始化参数

protected:
	CustomKeyboard keyboard;
	int Items;

	// 新增：电压值限制函数，统一处理电压最大值限制
	float limitVoltage(float voltage);
};

#endif // SETTINGVIEW_HPP
