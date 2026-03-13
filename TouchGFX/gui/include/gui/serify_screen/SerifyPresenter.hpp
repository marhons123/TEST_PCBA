#ifndef SERIFYPRESENTER_HPP
#define SERIFYPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SerifyView;

class SerifyPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SerifyPresenter(SerifyView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~SerifyPresenter() {}

private:
    SerifyPresenter();

    SerifyView& view;
};

#endif // SERIFYPRESENTER_HPP
