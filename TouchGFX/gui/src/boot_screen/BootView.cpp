#include <gui/boot_screen/BootView.hpp>

BootView::BootView()
{
	count = 0;
}

void BootView::setupScreen()
{
    BootViewBase::setupScreen();
}

void BootView::tearDownScreen()
{
    BootViewBase::tearDownScreen();
}

void BootView::handleTickEvent()
{
	count++;
	if(count % 50 == 0)
	{
	application().gotoTestScreenNoTransition();
	}
}
