#include <gui/test_screen/TestView.hpp>
#include <touchgfx/Color.hpp>
#include "cmsis_os.h"

extern osSemaphoreId testStartSemaphoreHandle;
extern osSemaphoreId clearDisplaySemaphoreHandle;

TestView::TestView() {

}

void TestView::setupScreen() {
	TestViewBase::setupScreen();
	loadParametersFromNorFlash();
	FRESULT res = loadDataFromNorFlash("0:/saveparam.bin", &saveparameterNumber,
			sizeof(saveparameterNumber), 1);
	number = saveparameterNumber;
	clearTestDisplay();
}

void TestView::tearDownScreen() {
	TestViewBase::tearDownScreen();
}

void TestView::handleTickEvent() {
    // 非阻塞方式检查信号量
    if (osSemaphoreAcquire(clearDisplaySemaphoreHandle, 0) == osOK) {
        clearTestDisplay();  // 清空UI界面
    }

    testShow();  // 正常的UI刷新函数
}

void TestView::testingClicked() {

	currentResult.testFinished = false;
	clearTestDisplay();  // 清空界面
	osSemaphoreRelease(testStartSemaphoreHandle);  // 触发测试任务
}

// 显示测试参数及其合格状态（颜色指示）
void TestView::testShow() {
	// 1. 显示产品编号（参数编号）
	Unicode::snprintf(parameterNumberBuffer, PARAMETERNUMBER_SIZE, "%d",
			parameters[number].parameterNumber);
	parameterNumber.setVisible(true);

	// 2. 显示产品型号（结构体中的Unicode字符拷贝）
	readCharFromBuffer(productModelBuffer, parameters[number].productModel,
			PRODUCTMODEL_SIZE);
	productModel.setVisible(true);

	// 3. 设置颜色并显示各项结果（红色NG，绿色OK）

	startingCoilVoltage.setColor(
			currentResult.startingCoilVoltageNG ?
					touchgfx::Color::getColorFromRGB(255, 0, 0) :
					touchgfx::Color::getColorFromRGB(0, 255, 0));
	startingCoilVoltage.setVisible(true);

//        startingTransitionTime.setColor(
//        currentResult.startingTransitionTimeNG ?
//            touchgfx::Color::getColorFromRGB(255, 0, 0) :
//            touchgfx::Color::getColorFromRGB(0, 255, 0));
//    startingTransitionTime.setVisible(true);

	startingTransitionOK.setVisible(currentResult.startingCoiltestFinished &&!currentResult.startingTransitionNG);
	startingTransitionNG.setVisible(currentResult.startingCoiltestFinished &&currentResult.startingTransitionNG);
//	startingTransitionOK.setVisible(currentResult.startingTransitionNG);
//	startingTransitionNG.setVisible(!currentResult.startingTransitionNG);


	holdingCoilVoltage.setColor(
			currentResult.holdingCoilVoltageNG ?
					touchgfx::Color::getColorFromRGB(255, 0, 0) :
					touchgfx::Color::getColorFromRGB(0, 255, 0));
	holdingCoilVoltage.setVisible(true);

	holdingVoltage1.setColor(
			currentResult.holdingVoltage1NG ?
					touchgfx::Color::getColorFromRGB(255, 0, 0) :
					touchgfx::Color::getColorFromRGB(0, 255, 0));
	holdingVoltage1.setVisible(true);

//	holdingVoltage2.setColor(
//			currentResult.holdingVoltage2NG ?
//					touchgfx::Color::getColorFromRGB(255, 0, 0) :
//					touchgfx::Color::getColorFromRGB(0, 255, 0));
//	holdingVoltage2.setVisible(true);

	holdingVoltage3.setColor(
			currentResult.holdingVoltage3NG ?
					touchgfx::Color::getColorFromRGB(255, 0, 0) :
					touchgfx::Color::getColorFromRGB(0, 255, 0));
	holdingVoltage3.setVisible(true);

	// 4. 设置各项数值文本显示

	Unicode::snprintfFloat(startingCoilVoltageBuffer, STARTINGCOILVOLTAGE_SIZE,
			"%0.2f", parameters[number].startingCoilVoltage);

	Unicode::snprintf(startingTransitionTimeBuffer, STARTINGTRANSITIONTIME_SIZE,
			"%0d", parameters[number].startingTransitionTime);

	Unicode::snprintfFloat(holdingCoilVoltageBuffer, HOLDINGCOILVOLTAGE_SIZE,
			"%0.2f", parameters[number].holdingCoilVoltage);

	Unicode::snprintfFloat(holdingVoltage1Buffer, HOLDINGVOLTAGE1_SIZE, "%0.2f",
			parameters[number].holdingVoltage1);

//	Unicode::snprintfFloat(holdingVoltage2Buffer, HOLDINGVOLTAGE2_SIZE, "%0.2f",
//			parameters[number].holdingVoltage2);

	Unicode::snprintfFloat(holdingVoltage3Buffer, HOLDINGVOLTAGE3_SIZE, "%0.2f",
			parameters[number].holdingVoltage3);

	Unicode::snprintf(totalProductionBuffer, TOTALPRODUCTION_SIZE, "%d",
			productionCount.totalProduction);
	totalProduction.setVisible(true);

	Unicode::snprintf(totalQualifiedBuffer, TOTALQUALIFIED_SIZE, "%d",
			productionCount.totalQualified);
	totalQualified.setVisible(true);

	Unicode::snprintf(totalDefectiveBuffer, TOTALDEFECTIVE_SIZE, "%d",
			productionCount.totalDefective);
	totalDefective.setVisible(true);

	// 5. 控制 OK / NG 显示逻辑
	OK.setVisible(currentResult.testFinished && currentResult.testPassed);
	NG.setVisible(currentResult.testFinished && !currentResult.testPassed);

	// 6. 最后刷新界面
	invalidate();
}

void TestView::clearTestDisplay() {
	// 隐藏测试值显示文本
	startingCoilVoltage.setVisible(false);
	startingTransitionTime.setVisible(false);
	holdingCoilVoltage.setVisible(false);
	holdingVoltage1.setVisible(false);
//	holdingVoltage2.setVisible(false);
	holdingVoltage3.setVisible(false);
	parameters[number].startingCoilVoltage = 0;
	parameters[number].startingTransitionTime = 0;
	parameters[number].holdingCoilVoltage = 0;
	parameters[number].holdingVoltage1 = 0;
//	parameters[number].holdingVoltage2 = 0;
	parameters[number].holdingVoltage3 = 0;

	// 隐藏统计数量
//    totalProduction.setVisible(false);
//    totalQualified.setVisible(false);
//    totalDefective.setVisible(false);

	// 隐藏结果标志
	OK.setVisible(false);
	NG.setVisible(false);

	startingTransitionOK.setVisible(false);
	startingTransitionNG.setVisible(false);

	invalidate();  // 刷新界面
}
