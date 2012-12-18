#include "basebutton.h"

BaseButton::BaseButton()
{
    radius = 40;
}

BaseButton::~BaseButton()
{
    removeListeners();
}

//add listener stuff
void BaseButton::addListeners()
{
    cout << "BaseButton::addListeners()" << endl;
}

//remove listener stuff
void BaseButton::removeListeners()
{

}

void BaseButton::_draw(ofEventArgs &e)
{
   draw();
}

void BaseButton::draw()
{

}

