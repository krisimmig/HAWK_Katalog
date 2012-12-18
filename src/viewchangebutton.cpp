#include "viewchangebutton.h"
//
ViewChangeButton::ViewChangeButton(std::string l, enum_views t, int x, int y)
{
    //ctor
    label = l;
    targetView = t;
    xPos = x;
    yPos = y;

    width = 125;
    height = 35;
    actionDelay = 25;
    actionTimer = 0;
    hoovering = false;
    addListeners();
}

ViewChangeButton::~ViewChangeButton()
{
    ofRemoveListener(ofEvents().draw, this, &ViewChangeButton::draw);
    ofRemoveListener(ofEvents().mouseReleased, this, &ViewChangeButton::mouseReleased);
    ofRemoveListener(ofEvents().mouseMoved, this, &ViewChangeButton::mouseMoved);
}

//add listener stuff
void ViewChangeButton::addListeners()
{
    //app events
    ofAddListener(ofEvents().draw, this, &ViewChangeButton::draw);
    ofAddListener(ofEvents().mouseReleased, this, &ViewChangeButton::mouseReleased);
    ofAddListener(ofEvents().mouseMoved, this, &ViewChangeButton::mouseMoved);
}

void ViewChangeButton::draw(ofEventArgs &e)
{
    if(hoovering)
    {
        ofFill();
        ofSetColor(255,0,0);
        ofRect(xPos, yPos, width/actionDelay * actionTimer, height);
    }

    ofNoFill();
    ofSetColor(0, 0, 0);
    ofRect(xPos, yPos, width, height);
    ofDrawBitmapString(label, xPos + 15, yPos + 15);
}

void ViewChangeButton::mouseReleased(ofMouseEventArgs &args)
{
    if(args.x > xPos && args.x < xPos + width &&
            args.y > yPos && args.y < yPos + height )
    {
        changeView(targetView);
    }
}

void ViewChangeButton::mouseMoved(ofMouseEventArgs &args)
{
    if(hoverState(args.x, args.y) )
    {
        changeView(targetView);
    }
}

void ViewChangeButton::changeView(enum_views target)
{
    static CustomEvent changeViewEvent;
    changeViewEvent.view = target;
    ofNotifyEvent(CustomEvent::viewChange, changeViewEvent);
}

bool ViewChangeButton::hoverState(int x, int y)
{
    if( x > xPos && x < xPos + width &&
            y > yPos && y < yPos + height )
    {
        hoovering = true;
        actionTimer++;
        if(actionTimer > actionDelay)
        {
            actionTimer = 0;
            return true;
        }
        return false;
    }
    hoovering = false;
    actionTimer = 0;
    return false;
}
