#include "view.h"

View::View()
{
    //ctor
}

void View::setup(HandCursor & c, ViewManager & vs)
{
    // ---------------------------------
    // views and cursor setup
    // ---------------------------------
    cursor = &c;
    viewmanager = &vs;
    trackingHand = false;
    setupButtons();
}

void View::setupButtons()
{
    // gets called to initialize each views buttons etc.
}

void View::update(bool t)
{
    trackingHand = t;
}

View::~View()
{
    //dtor
}
