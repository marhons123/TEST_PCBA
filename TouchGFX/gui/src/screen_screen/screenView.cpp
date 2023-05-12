#include <gui/screen_screen/screenView.hpp>
#include <touchgfx/Unicode.hpp>

#ifndef SIMULATOR
#include <test.h>
#endif


screenView::screenView() {

}

void screenView::setupScreen() {
	screenViewBase::setupScreen();
}

void screenView::tearDownScreen() {
	screenViewBase::tearDownScreen();
}

void screenView::changeScalableImage1Size(float factor) {
	scalableImage1.setWidthHeight(scalableImage1.getWidth() * factor,
			scalableImage1.getHeight() * factor);
	//scalableImage1.invalidate();
	invalidate();
}

void screenView::smallerButtonClickHandler() {
	changeScalableImage1Size(0.9f);
}

void screenView::baggerButtonClickHandler() {
	changeScalableImage1Size(1.1f);
}

void screenView::changeButtonClickHandler() {
	if (scalableImage1.getScalingAlgorithm() == ScalableImage::NEAREST_NEIGHBOR)
		scalableImage1.setScalingAlgorithm(
				ScalableImage::BILINEAR_INTERPOLATION);
	else
		scalableImage1.setScalingAlgorithm(ScalableImage::NEAREST_NEIGHBOR);
	scalableImage1.invalidate();
}

void screenView::handleTickEvent() {
	moveIamge();
}

void screenView::moveIamge() {
	static uint16_t tick = 0;
	int newY;

	tick++;
	if (tick == 3) {
		tick = 0;

		if (image2.getY() <= (-image2.getHeight())) //说明正好全部出去了
			newY = image2.getParent()->getHeight();

		else
			newY = image2.getY() - 5;

		image2.setY(newY);
#ifndef SIMULATOR
		Unicode::snprintf(pullTextBuffer,PULLTEXT_SIZE,"%d",num);
#endif
		invalidate();
	}
}




