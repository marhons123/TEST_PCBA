#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Unicode.hpp>

#ifndef SIMULATOR
#include <adc.h>
#endif

Screen1View::Screen1View() {

}

void Screen1View::setupScreen() {
	Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen() {
	Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent() {
	adcShow();
}

void Screen1View::adcShow() {

#ifndef SIMULATOR
	Unicode::snprintf(textArea2Buffer,TEXTAREA2_SIZE, "%d", val);
	Unicode::snprintfFloat(textArea3Buffer,TEXTAREA3_SIZE, "%0.2f", volt);
	Unicode::snprintfFloat(textArea5Buffer,TEXTAREA5_SIZE, "%0.2f", vrefint);
	//Unicode::snprintfFloat(textArea6Buffer,TEXTAREA6_SIZE, "%0.2f", vbat);
	Unicode::snprintfFloat(textArea6Buffer,TEXTAREA6_SIZE, "%0.2f", temperate);
#endif
	invalidate();
}

