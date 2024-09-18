#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Unicode.hpp>


#ifndef SIMULATOR
#include <adc.h>
#include <dac.h>
#endif


Screen1View::Screen1View() {
	tickCounter = 0;
}

void Screen1View::setupScreen() {
	Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen() {
	Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent() {
	adcShow();
//	temperatureGauge();
}

void Screen1View::adcShow() {

#ifndef SIMULATOR
    Unicode::snprintf(textArea2Buffer,TEXTAREA2_SIZE, "%d", value);
    Unicode::snprintfFloat(textArea3Buffer,TEXTAREA3_SIZE, "%0.3f", voltageIn5);
	Unicode::snprintfFloat(textArea5Buffer,TEXTAREA5_SIZE, "%0.3f", vrefintVoltage);
	//Unicode::snprintfFloat(textArea6Buffer,TEXTAREA6_SIZE, "%0.2f", vbat);
	Unicode::snprintfFloat(textArea6Buffer,TEXTAREA6_SIZE, "%0.2f", temperature);

	Unicode::snprintf(textArea7Buffer,TEXTAREA7_SIZE, "%d", dacValue);
	Unicode::snprintfFloat(textArea8Buffer,TEXTAREA8_SIZE, "%0.3f", dacOut1);

#endif

	readCharFromBuffer(messages1Buffer, messagesText1, MESSAGES1_SIZE);
	readCharFromBuffer(messages2Buffer, messagesText2, MESSAGES2_SIZE);
	readCharFromBuffer(messages3Buffer, messagesText3, MESSAGES3_SIZE);

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



