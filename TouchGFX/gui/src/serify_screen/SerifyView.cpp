#include <gui/serify_screen/SerifyView.hpp>
#include <touchgfx/Unicode.hpp>

#ifndef SIMULATOR
#include <adc.h>
#include <dac.h>
#include "test.h"
#include "cmsis_os.h"
#endif
extern osSemaphoreId testStartSemaphoreHandle;

SerifyView::SerifyView()
{
	tickCounter = 0;
	keyboard.setPosition(155, 60, 640, 480);
	add(keyboard);
	keyboard.setVisible(false);
	keyboard.invalidate();
}

void SerifyView::setupScreen()
{
    SerifyViewBase::setupScreen();

    // 恢复 Toggle 状态
    toggleButton.forceState(isToggleOn);
    toggleButton.invalidate();
}

void SerifyView::tearDownScreen()
{
    SerifyViewBase::tearDownScreen();
    dacOut1 = 0.0f;
    dacValue = voltageToDAC(dacOut1);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue); // 设置DAC输出
	HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_SET); // 关闭电压输出
}

void SerifyView::handleTickEvent() {
	adcShow();
//	temperatureGauge();
}

void SerifyView::testingClicked() {
	osSemaphoreRelease(testStartSemaphoreHandle);
}

void SerifyView::keyBoardShow() {
	keyboard.setVisible(true);  // show keyboard
	keyboard.invalidate();
	flexButtonOK.setVisible(true);  // show OK
	flexButtonOK.invalidate();
	flexButtonUp.setVisible(true);  // show OK
	flexButtonUp.invalidate();
	flexButtonDown.setVisible(true);  // show OK
	flexButtonDown.invalidate();
	flexButtonExit.setVisible(true);  // show exit
	flexButtonExit.invalidate();
}

void SerifyView::keyBoardHide() {
	keyboard.setVisible(false);  // hide keyboard
	keyboard.invalidate();
	flexButtonOK.setVisible(false);  // hide OK
	flexButtonOK.invalidate();
	flexButtonUp.setVisible(false);  // show OK
	flexButtonUp.invalidate();
	flexButtonDown.setVisible(false);  // show OK
	flexButtonDown.invalidate();
	flexButtonExit.setVisible(false);  // hide exit
	flexButtonExit.invalidate();
}

void SerifyView::outputVoltageClicked() {
	keyBoardShow();
	Items = 1;
}

void SerifyView::toggleButtonClicked()
{
    if (isToggleOn) {
        // 关闭
        dacOut1 = 0.0f;
        dacValue = voltageToDAC(dacOut1);
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue);
        HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_SET);
    } else {
        // 打开
        dacOut1 = convertToFloat(outputVoltageAreaBuffer, OUTPUTVOLTAGEAREA_SIZE);
        dacValue = voltageToDAC(dacOut1);
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue);
        HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_RESET);
    }

    isToggleOn = !isToggleOn;
    toggleButton.forceState(isToggleOn);  // 同步 UI 状态
    toggleButton.invalidate();
}

void SerifyView::exitClicked() {

	keyBoardHide();
}

void SerifyView::okClicked() {
	keyBoardHide();

	if (Items == 1) {
		Unicode::strncpy(outputVoltageAreaBuffer, keyboard.getBuffer(),
				OUTPUTVOLTAGEAREA_SIZE);  //显示键盘输入字符
		outputVoltageArea.invalidate();
		Items = 0;
	}
}

void SerifyView::adcShow() {

#ifndef SIMULATOR
	Unicode::snprintf(textArea2Buffer, TEXTAREA2_SIZE, "%d", value1);
	Unicode::snprintfFloat(textArea3Buffer, TEXTAREA3_SIZE, "%0.3f",
			voltageIn5);
	Unicode::snprintfFloat(textArea5Buffer, TEXTAREA5_SIZE, "%0.3f",
			parameters[number].holdingVoltage3);
//	Unicode::snprintfFloat(textArea5Buffer,TEXTAREA5_SIZE, "%0.3f", vrefintVoltage);
//	Unicode::snprintfFloat(textArea6Buffer,TEXTAREA6_SIZE, "%0.2f", vbat);
	Unicode::snprintfFloat(textArea6Buffer, TEXTAREA6_SIZE, "%0.2f",
			temperature);

	Unicode::snprintf(textArea7Buffer, TEXTAREA7_SIZE, "%d", dacValue);
	Unicode::snprintfFloat(textArea8Buffer, TEXTAREA8_SIZE, "%0.3f", dacOut1);

#endif

//	readCharFromBuffer(messages1Buffer, messagesText1, MESSAGES1_SIZE);
//	readCharFromBuffer(messages2Buffer, messagesText2, MESSAGES2_SIZE);
//	readCharFromBuffer(messages3Buffer, messagesText3, MESSAGES3_SIZE);

	invalidate();
}

//void Screen1View::temperatureGauge()
//{
//    tickCounter++;
//
//    // Change value every 120 tick.
//    if (tickCounter % 120 == 0)
//    {
//      // Insert data point
//#ifndef SIMULATOR
//    gauge1.updateValue(temperature, 30);
//#endif

// animates needle and arc to new value with a duration of 30 ticks
//    }
//}


