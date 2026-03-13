#ifndef SERIFYVIEW_HPP
#define SERIFYVIEW_HPP

#include <gui_generated/serify_screen/SerifyViewBase.hpp>
#include <gui/serify_screen/SerifyPresenter.hpp>
#include "gui/common/TestVariables.hpp"
#include <gui/common/CustomKeyboard.hpp>
#include "test.h"

class SerifyView : public SerifyViewBase
{
public:
    SerifyView();
    virtual ~SerifyView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
	virtual void testingClicked();
	virtual void okClicked();
	virtual void exitClicked();
	virtual void keyBoardShow();
	virtual void keyBoardHide();
	virtual void outputVoltageClicked();
	virtual void toggleButtonClicked();
	void adcShow();
	int tickCounter;
//    void temperatureGauge();
protected:
	 CustomKeyboard keyboard;
	 int Items;
private:
    bool isToggleOn = false;
};

#endif // SERIFYVIEW_HPP
