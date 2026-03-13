#ifndef TESTVIEW_HPP
#define TESTVIEW_HPP

#include <gui_generated/test_screen/TestViewBase.hpp>
#include <gui/test_screen/TestPresenter.hpp>
#include "gui/common/TestVariables.hpp"
#include "test.h"
#include "norflashmanager.h"
#include "stdbool.h"

class TestView : public TestViewBase
{
public:
    TestView();
    virtual ~TestView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void testingClicked();
    void testShow();
    void clearTestDisplay();

protected:
};

#endif // TESTVIEW_HPP
