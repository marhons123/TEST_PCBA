#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP

#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

class screenView: public screenViewBase {
public:
	screenView();
	virtual ~screenView() {
	}
	virtual void setupScreen();
	virtual void tearDownScreen();
	void changeScalableImage1Size(float factor);
	virtual void smallerButtonClickHandler();
	virtual void baggerButtonClickHandler();
	virtual void changeButtonClickHandler();
	virtual void handleTickEvent();
	void moveIamge();
	int tickCounter;
	void dynamicGraph();
	int tickCounter1;
	int analogHours;
	int analogMinutes;
	int analogSeconds;
	void analogClock();
	int tickCounter2;
	int digitalHours;
	int digitalMinutes;
	int digitalSeconds;
	void digitalClock();



protected:


};

#endif // SCREENVIEW_HPP
