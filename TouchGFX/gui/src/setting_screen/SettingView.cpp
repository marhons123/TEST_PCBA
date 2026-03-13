#include <gui/setting_screen/SettingView.hpp>

SettingView::SettingView() {
	keyboard.setPosition(155, 60, 640, 480);
	add(keyboard);
	keyboard.setVisible(false);
	keyboard.invalidate();
}

void SettingView::setupScreen() {
	SettingViewBase::setupScreen();
	loadParameters();//更新数据
}

void SettingView::tearDownScreen() {
	SettingViewBase::tearDownScreen();
	saveparameterNumber = parameters[number].parameterNumber;
	FRESULT res = saveDataToNorFlash("0:/saveparam.bin", &saveparameterNumber, sizeof(saveparameterNumber), 1);
	if (res == FR_OK) {
		printf("saveparameterNumber saved successfully.\n");
	} else {
		printf("Failed to save saveparameterNumber. Error: %d\n", res);
	}
	saveParametersToNorFlash();
}

void SettingView:: resetCounterClicked() {
    memset(&productionCount, 0, sizeof(Statistics));	//计数全部清零
}

void SettingView:: parameterNumberClicked() {
	keyBoardShow();
	Items = 1;
}

void SettingView::productModelClicked() {
	keyBoardShow();
	Items = 2;
}

void SettingView:: ratedVoltageClicked() {
	keyBoardShow();
	Items = 3;
}

void SettingView:: samplingTimeClicked() {
	keyBoardShow();
	Items = 4;
}

void SettingView:: inputVoltage1Clicked() {
	keyBoardShow();
	Items = 5;
}

void SettingView:: inputVoltage2Clicked() {
	keyBoardShow();
	Items = 6;
}

void SettingView:: inputVoltage3Clicked() {
	keyBoardShow();
	Items = 7;
}

void SettingView:: startingCoilVoltageUpperLimitClicked() {
	keyBoardShow();
	Items = 8;
}

void SettingView:: startingCoilVoltageLowerLimitClicked() {
	keyBoardShow();
	Items = 9;
}

void SettingView:: startingTransitionTimeUpperLimitClicked() {
	keyBoardShow();
	Items = 10;
}

void SettingView:: startingTransitionTimeLowerLimitClicked() {
	keyBoardShow();
	Items = 11;
}

void SettingView:: holdingCoilVoltageUpperLimitClicked() {
	keyBoardShow();
	Items =12;
}

void SettingView:: holdingCoilVoltageLowerLimitClicked() {
	keyBoardShow();
	Items = 13;
}

void SettingView:: holdingVoltage1UpperLimitClicked() {
	keyBoardShow();
	Items = 14;
}

void SettingView:: holdingVoltage1LowerLimitClicked() {
	keyBoardShow();
	Items = 15;
}

void SettingView:: holdingVoltage2UpperLimitClicked() {
	keyBoardShow();
	Items = 16;
}

void SettingView:: holdingVoltage2LowerLimitClicked() {
	keyBoardShow();
	Items = 17;
}

void SettingView:: holdingVoltage3UpperLimitClicked() {
	keyBoardShow();
	Items = 18;
}

void SettingView:: holdingVoltage3LowerLimitClicked() {
	keyBoardShow();
	Items = 19;
}

void SettingView::exitClicked() {
	keyBoardHide();
}

// 电压值限制函数实现
float SettingView::limitVoltage(float voltage) {
    // 如果电压超过12V，返回12V；否则返回原值
    return (voltage > MAX_VOLTAGE_LIMIT) ? MAX_VOLTAGE_LIMIT : voltage;
}

void SettingView::okClicked() {
	keyBoardHide();

	if (Items == 1) {
		Unicode::strncpy(parameterNumberBuffer, keyboard.getBuffer(),PARAMETERNUMBER_SIZE);
		parameterNumber.invalidate();
		number = convertToUint16(keyboard.getBuffer(), RATEDVOLTAGE_SIZE);
		parameters[number].parameterNumber = number;
		loadParameters();
		Items = 0;
	}

	if (Items == 2) {
		Unicode::strncpy(productModelBuffer, keyboard.getBuffer(),PRODUCTMODEL_SIZE);
		productModel.invalidate();
		storeCharToBuffer(productModelBuffer, parameters[number].productModel, PRODUCTMODEL_SIZE);
		Items = 0;
	}

	if (Items == 3 ) {
		Unicode::strncpy(ratedVoltageBuffer, keyboard.getBuffer(),RATEDVOLTAGE_SIZE);
		ratedVoltage.invalidate();
		// 限制额定电压不超过12V
		float ratedVoltage = convertToFloat(keyboard.getBuffer(), RATEDVOLTAGE_SIZE);
		parameters[number].ratedVoltage = limitVoltage(ratedVoltage);
		// 同步更新显示缓冲区的值
		Unicode::snprintfFloat(ratedVoltageBuffer, RATEDVOLTAGE_SIZE, "%.2f", parameters[number].ratedVoltage);
		Items = 0;
	}

	if (Items == 4 ) {
		Unicode::strncpy(samplingTimeBuffer, keyboard.getBuffer(),SAMPLINGTIME_SIZE);
		samplingTime.invalidate();
		parameters[number].samplingTime = convertToFloat(keyboard.getBuffer(), SAMPLINGTIME_SIZE);
		Items = 0;
	}

	if (Items == 5 ) {
		Unicode::strncpy(inputVoltage1Buffer, keyboard.getBuffer(),INPUTVOLTAGE1_SIZE);
		inputVoltage1.invalidate();
		// 限制输入电压1不超过12V
		float inputVoltage1 = convertToFloat(keyboard.getBuffer(), INPUTVOLTAGE1_SIZE);
		parameters[number].inputVoltage1 = limitVoltage(inputVoltage1);
		Unicode::snprintfFloat(inputVoltage1Buffer, INPUTVOLTAGE1_SIZE, "%.2f", parameters[number].inputVoltage1);
		Items = 0;
	}

	if (Items == 6 ) {
		Unicode::strncpy(inputVoltage2Buffer, keyboard.getBuffer(),INPUTVOLTAGE2_SIZE);
		inputVoltage2.invalidate();
		// 限制输入电压2不超过12V
		float inputVoltage2 = convertToFloat(keyboard.getBuffer(), INPUTVOLTAGE2_SIZE);
		parameters[number].inputVoltage2 = limitVoltage(inputVoltage2);
		Unicode::snprintfFloat(inputVoltage2Buffer, INPUTVOLTAGE2_SIZE, "%.2f", parameters[number].inputVoltage2);
		Items = 0;
	}

	if (Items == 7 ) {
		Unicode::strncpy(inputVoltage3Buffer, keyboard.getBuffer(),INPUTVOLTAGE3_SIZE);
		inputVoltage3.invalidate();
		// 限制输入电压3不超过12V
		float inputVoltage3 = convertToFloat(keyboard.getBuffer(), INPUTVOLTAGE3_SIZE);
		parameters[number].inputVoltage3 = limitVoltage(inputVoltage3);
		Unicode::snprintfFloat(inputVoltage3Buffer, INPUTVOLTAGE3_SIZE, "%.2f", parameters[number].inputVoltage3);
		Items = 0;
	}

	if (Items == 8 ) {
		Unicode::strncpy(startingCoilVoltageUpperLimitBuffer, keyboard.getBuffer(),STARTINGCOILVOLTAGEUPPERLIMIT_SIZE);
		startingCoilVoltageUpperLimit.invalidate();
		// 限制启动线圈电压上限不超过12V
		float startCoilUpper = convertToFloat(keyboard.getBuffer(), STARTINGCOILVOLTAGEUPPERLIMIT_SIZE);
		parameters[number].startingCoilVoltageUpperLimit = limitVoltage(startCoilUpper);
		Unicode::snprintfFloat(startingCoilVoltageUpperLimitBuffer, STARTINGCOILVOLTAGEUPPERLIMIT_SIZE, "%.2f", parameters[number].startingCoilVoltageUpperLimit);
		Items = 0;
	}

	if (Items == 9 ) {
		Unicode::strncpy(startingCoilVoltageLowerLimitBuffer, keyboard.getBuffer(),STARTINGCOILVOLTAGELOWERLIMIT_SIZE);
		startingCoilVoltageLowerLimit.invalidate();
		// 限制启动线圈电压下限不超过12V
		float startCoilLower = convertToFloat(keyboard.getBuffer(), STARTINGCOILVOLTAGELOWERLIMIT_SIZE);
		parameters[number].startingCoilVoltageLowerLimit = limitVoltage(startCoilLower);
		Unicode::snprintfFloat(startingCoilVoltageLowerLimitBuffer, STARTINGCOILVOLTAGELOWERLIMIT_SIZE, "%.2f", parameters[number].startingCoilVoltageLowerLimit);
		Items = 0;
	}

	if (Items == 10 ) {
		Unicode::strncpy(startingTransitionTimeUpperLimitBuffer, keyboard.getBuffer(),STARTINGTRANSITIONTIMEUPPERLIMIT_SIZE);
		startingTransitionTimeUpperLimit.invalidate();
		parameters[number].startingTransitionTimeUpperLimit = convertToFloat(keyboard.getBuffer(), STARTINGTRANSITIONTIMEUPPERLIMIT_SIZE);
		Items = 0;
	}

	if (Items == 11 ) {
		Unicode::strncpy(startingTransitionTimeLowerLimitBuffer, keyboard.getBuffer(),STARTINGTRANSITIONTIMELOWERLIMIT_SIZE);
		startingTransitionTimeLowerLimit.invalidate();
		parameters[number].startingTransitionTimeLowerLimit = convertToFloat(keyboard.getBuffer(), STARTINGTRANSITIONTIMELOWERLIMIT_SIZE);
		Items = 0;
	}

	if (Items == 12 ) {
		Unicode::strncpy(holdingCoilVoltageUpperLimitBuffer, keyboard.getBuffer(),HOLDINGCOILVOLTAGEUPPERLIMIT_SIZE);
		holdingCoilVoltageUpperLimit.invalidate();
		// 限制保持线圈电压上限不超过12V
		float holdCoilUpper = convertToFloat(keyboard.getBuffer(), HOLDINGCOILVOLTAGEUPPERLIMIT_SIZE);
		parameters[number].holdingCoilVoltageUpperLimit = limitVoltage(holdCoilUpper);
		Unicode::snprintfFloat(holdingCoilVoltageUpperLimitBuffer, HOLDINGCOILVOLTAGEUPPERLIMIT_SIZE, "%.2f", parameters[number].holdingCoilVoltageUpperLimit);
		Items = 0;
	}

	if (Items == 13 ) {
		Unicode::strncpy(holdingCoilVoltageLowerLimitBuffer, keyboard.getBuffer(),HOLDINGCOILVOLTAGELOWERLIMIT_SIZE);
		holdingCoilVoltageLowerLimit.invalidate();
		// 限制保持线圈电压下限不超过12V
		float holdCoilLower = convertToFloat(keyboard.getBuffer(), HOLDINGCOILVOLTAGELOWERLIMIT_SIZE);
		parameters[number].holdingCoilVoltageLowerLimit = limitVoltage(holdCoilLower);
		Unicode::snprintfFloat(holdingCoilVoltageLowerLimitBuffer, HOLDINGCOILVOLTAGELOWERLIMIT_SIZE, "%.2f", parameters[number].holdingCoilVoltageLowerLimit);
		Items = 0;
	}

	if (Items == 14 ) {
		Unicode::strncpy(holdingVoltage1UpperLimitBuffer, keyboard.getBuffer(),HOLDINGVOLTAGE1UPPERLIMIT_SIZE);
		holdingVoltage1UpperLimit.invalidate();
		// 限制保持电压1上限不超过12V
		float holdVolt1Upper = convertToFloat(keyboard.getBuffer(), HOLDINGVOLTAGE1UPPERLIMIT_SIZE);
		parameters[number].holdingVoltage1UpperLimit = limitVoltage(holdVolt1Upper);
		Unicode::snprintfFloat(holdingVoltage1UpperLimitBuffer, HOLDINGVOLTAGE1UPPERLIMIT_SIZE, "%.2f", parameters[number].holdingVoltage1UpperLimit);
		Items = 0;
	}

	if (Items == 15 ) {
		Unicode::strncpy(holdingVoltage1LowerLimitBuffer, keyboard.getBuffer(),HOLDINGVOLTAGE1LOWERLIMIT_SIZE);
		holdingVoltage1LowerLimit.invalidate();
		// 限制保持电压1下限不超过12V
		float holdVolt1Lower = convertToFloat(keyboard.getBuffer(), HOLDINGVOLTAGE1LOWERLIMIT_SIZE);
		parameters[number].holdingVoltage1LowerLimit = limitVoltage(holdVolt1Lower);
		Unicode::snprintfFloat(holdingVoltage1LowerLimitBuffer, HOLDINGVOLTAGE1LOWERLIMIT_SIZE, "%.2f", parameters[number].holdingVoltage1LowerLimit);
		Items = 0;
	}

	if (Items == 16 ) {
		Unicode::strncpy(holdingVoltage2UpperLimitBuffer, keyboard.getBuffer(),HOLDINGVOLTAGE2UPPERLIMIT_SIZE);
		holdingVoltage2UpperLimit.invalidate();
		// 限制保持电压2上限不超过12V
		float holdVolt2Upper = convertToFloat(keyboard.getBuffer(), HOLDINGVOLTAGE2UPPERLIMIT_SIZE);
		parameters[number].holdingVoltage2UpperLimit = limitVoltage(holdVolt2Upper);
		Unicode::snprintfFloat(holdingVoltage2UpperLimitBuffer, HOLDINGVOLTAGE2UPPERLIMIT_SIZE, "%.2f", parameters[number].holdingVoltage2UpperLimit);
		Items = 0;
	}

	if (Items == 17 ) {
		Unicode::strncpy(holdingVoltage2LowerLimitBuffer, keyboard.getBuffer(),HOLDINGVOLTAGE2LOWERLIMIT_SIZE);
		holdingVoltage2LowerLimit.invalidate();
		// 限制保持电压2下限不超过12V
		float holdVolt2Lower = convertToFloat(keyboard.getBuffer(), HOLDINGVOLTAGE2LOWERLIMIT_SIZE);
		parameters[number].holdingVoltage2LowerLimit = limitVoltage(holdVolt2Lower);
		Unicode::snprintfFloat(holdingVoltage2LowerLimitBuffer, HOLDINGVOLTAGE2LOWERLIMIT_SIZE, "%.2f", parameters[number].holdingVoltage2LowerLimit);
		Items = 0;
	}

	if (Items == 18 ) {
		Unicode::strncpy(holdingVoltage3UpperLimitBuffer, keyboard.getBuffer(),HOLDINGVOLTAGE3UPPERLIMIT_SIZE);
		holdingVoltage3UpperLimit.invalidate();
		// 限制保持电压3上限不超过12V
		float holdVolt3Upper = convertToFloat(keyboard.getBuffer(), HOLDINGVOLTAGE3UPPERLIMIT_SIZE);
		parameters[number].holdingVoltage3UpperLimit = limitVoltage(holdVolt3Upper);
		Unicode::snprintfFloat(holdingVoltage3UpperLimitBuffer, HOLDINGVOLTAGE3UPPERLIMIT_SIZE, "%.2f", parameters[number].holdingVoltage3UpperLimit);
		Items = 0;
	}

	if (Items == 19 ) {
		Unicode::strncpy(holdingVoltage3LowerLimitBuffer, keyboard.getBuffer(),HOLDINGVOLTAGE3LOWERLIMIT_SIZE);
		holdingVoltage3LowerLimit.invalidate();
		// 限制保持电压3下限不超过12V
		float holdVolt3Lower = convertToFloat(keyboard.getBuffer(), HOLDINGVOLTAGE3LOWERLIMIT_SIZE);
		parameters[number].holdingVoltage3LowerLimit = limitVoltage(holdVolt3Lower);
		Unicode::snprintfFloat(holdingVoltage3LowerLimitBuffer, HOLDINGVOLTAGE3LOWERLIMIT_SIZE, "%.2f", parameters[number].holdingVoltage3LowerLimit);
		Items = 0;
	}

	keyboard.clearBuffer();
}

void SettingView::keyBoardShow()
{
	keyboard.setVisible(true);
	keyboard.invalidate();
	flexButtonOK.setVisible(true);
	flexButtonOK.invalidate();
	flexButtonUp.setVisible(true);
	flexButtonUp.invalidate();
	flexButtonDown.setVisible(true);
	flexButtonDown.invalidate();
	flexButtonExit.setVisible(true);
	flexButtonExit.invalidate();
}

void SettingView::keyBoardHide()
{
	keyboard.setVisible(false);
	keyboard.invalidate();
	flexButtonOK.setVisible(false);
	flexButtonOK.invalidate();
	flexButtonUp.setVisible(false);
	flexButtonUp.invalidate();
	flexButtonDown.setVisible(false);
	flexButtonDown.invalidate();
	flexButtonExit.setVisible(false);
	flexButtonExit.invalidate();
}

void SettingView::initializeParameters()
{
	// 保留原有初始化逻辑（如需启用，取消注释）
}

void SettingView::loadParameters()//从结构体更新参数
{
	Unicode::snprintf(parameterNumberBuffer, PARAMETERNUMBER_SIZE, "%d",
				parameters[number].parameterNumber);

	// 加载额定电压时也确保不超过12V
	float limitedRatedVoltage = limitVoltage(parameters[number].ratedVoltage);
	Unicode::snprintfFloat(ratedVoltageBuffer, RATEDVOLTAGE_SIZE, "%.2f", limitedRatedVoltage);

	readCharFromBuffer(productModelBuffer, parameters[number].productModel, PRODUCTMODEL_SIZE);

	Unicode::snprintf(samplingTimeBuffer, SAMPLINGTIME_SIZE, "%d",
					parameters[number].samplingTime);

	// 加载输入电压1时限制最大值
	float limitedInputVolt1 = limitVoltage(parameters[number].inputVoltage1);
	Unicode::snprintfFloat(inputVoltage1Buffer, INPUTVOLTAGE1_SIZE, "%.2f", limitedInputVolt1);

	// 加载输入电压2时限制最大值
	float limitedInputVolt2 = limitVoltage(parameters[number].inputVoltage2);
	Unicode::snprintfFloat(inputVoltage2Buffer, INPUTVOLTAGE2_SIZE, "%.2f", limitedInputVolt2);

	// 加载输入电压3时限制最大值
	float limitedInputVolt3 = limitVoltage(parameters[number].inputVoltage3);
	Unicode::snprintfFloat(inputVoltage3Buffer, INPUTVOLTAGE3_SIZE, "%.2f", limitedInputVolt3);

	// 加载启动线圈电压上限时限制最大值
	float limitedStartCoilUpper = limitVoltage(parameters[number].startingCoilVoltageUpperLimit);
	Unicode::snprintfFloat(startingCoilVoltageUpperLimitBuffer, STARTINGCOILVOLTAGEUPPERLIMIT_SIZE, "%.2f", limitedStartCoilUpper);

	// 加载启动线圈电压下限时限制最大值
	float limitedStartCoilLower = limitVoltage(parameters[number].startingCoilVoltageLowerLimit);
	Unicode::snprintfFloat(startingCoilVoltageLowerLimitBuffer, STARTINGCOILVOLTAGELOWERLIMIT_SIZE, "%.2f", limitedStartCoilLower);

	Unicode::snprintf(startingTransitionTimeUpperLimitBuffer, STARTINGTRANSITIONTIMEUPPERLIMIT_SIZE, "%d",
					parameters[number].startingTransitionTimeUpperLimit);

	Unicode::snprintf(startingTransitionTimeLowerLimitBuffer, STARTINGTRANSITIONTIMELOWERLIMIT_SIZE, "%d",
					parameters[number].startingTransitionTimeLowerLimit);

	// 加载保持线圈电压上限时限制最大值
	float limitedHoldCoilUpper = limitVoltage(parameters[number].holdingCoilVoltageUpperLimit);
	Unicode::snprintfFloat(holdingCoilVoltageUpperLimitBuffer, HOLDINGCOILVOLTAGEUPPERLIMIT_SIZE, "%.2f", limitedHoldCoilUpper);

	// 加载保持线圈电压下限时限制最大值
	float limitedHoldCoilLower = limitVoltage(parameters[number].holdingCoilVoltageLowerLimit);
	Unicode::snprintfFloat(holdingCoilVoltageLowerLimitBuffer, HOLDINGCOILVOLTAGELOWERLIMIT_SIZE, "%.2f", limitedHoldCoilLower);

	// 加载保持电压1上限时限制最大值
	float limitedHoldVolt1Upper = limitVoltage(parameters[number].holdingVoltage1UpperLimit);
	Unicode::snprintfFloat(holdingVoltage1UpperLimitBuffer, HOLDINGVOLTAGE1UPPERLIMIT_SIZE, "%.2f", limitedHoldVolt1Upper);

	// 加载保持电压1下限时限制最大值
	float limitedHoldVolt1Lower = limitVoltage(parameters[number].holdingVoltage1LowerLimit);
	Unicode::snprintfFloat(holdingVoltage1LowerLimitBuffer, HOLDINGVOLTAGE1LOWERLIMIT_SIZE, "%.2f", limitedHoldVolt1Lower);

	// 加载保持电压2上限时限制最大值
	float limitedHoldVolt2Upper = limitVoltage(parameters[number].holdingVoltage2UpperLimit);
	Unicode::snprintfFloat(holdingVoltage2UpperLimitBuffer, HOLDINGVOLTAGE2UPPERLIMIT_SIZE, "%.2f", limitedHoldVolt2Upper);

	// 加载保持电压2下限时限制最大值
	float limitedHoldVolt2Lower = limitVoltage(parameters[number].holdingVoltage2LowerLimit);
	Unicode::snprintfFloat(holdingVoltage2LowerLimitBuffer, HOLDINGVOLTAGE2LOWERLIMIT_SIZE, "%.2f", limitedHoldVolt2Lower);

	// 加载保持电压3上限时限制最大值
	float limitedHoldVolt3Upper = limitVoltage(parameters[number].holdingVoltage3UpperLimit);
	Unicode::snprintfFloat(holdingVoltage3UpperLimitBuffer, HOLDINGVOLTAGE3UPPERLIMIT_SIZE, "%.2f", limitedHoldVolt3Upper);

	// 加载保持电压3下限时限制最大值
	float limitedHoldVolt3Lower = limitVoltage(parameters[number].holdingVoltage3LowerLimit);
	Unicode::snprintfFloat(holdingVoltage3LowerLimitBuffer, HOLDINGVOLTAGE3LOWERLIMIT_SIZE, "%.2f", limitedHoldVolt3Lower);

	invalidate();
}
