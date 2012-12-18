#include "button.h"

RoundButton::RoundButton(std::string l, std::string d, int x, int y)
{
    //ctor
    label = l;
    dept = d;
    xPos = x;
    yPos = y;

    radius = 40;
    actionTeaDelay = 25;
    actionTeaTimer = 0;
    hoovering = false;
    addListeners();
}

RoundButton::~RoundButton()
{
    ofRemoveListener(ofEvents().draw, this, &RoundButton::draw);
    ofRemoveListener(ofEvents().mouseReleased, this, &RoundButton::mouseReleased);
    ofRemoveListener(ofEvents().mouseMoved, this, &RoundButton::mouseMoved);
}

//add listener stuff
void RoundButton::addListeners()
{
    //app events
    ofAddListener(ofEvents().draw, this, &RoundButton::draw);
    ofAddListener(ofEvents().mouseReleased, this, &RoundButton::mouseReleased);
    ofAddListener(ofEvents().mouseMoved, this, &RoundButton::mouseMoved);
}

void RoundButton::draw(ofEventArgs &e)
{
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofCircle(xPos, yPos, radius);
    ofDrawBitmapString(label, xPos - 20, yPos + 5);
    if(hoovering)
    {
        // count down graphic
        ofPath timerShape;
        timerShape.setFilled(false);
        timerShape.setStrokeWidth(10);
        timerShape.setStrokeColor(ofColor(255, 0, 0));
        timerShape.arc(xPos, yPos, radius, radius, 0, (360/actionTeaDelay) * actionTeaTimer);
        timerShape.draw();
    }
}

void RoundButton::mouseMoved(ofMouseEventArgs &args)
{
    if(hoverState(args.x, args.y) )
    {
        changeDept(dept);
        changeViewToOverview();
    }
}

void RoundButton::mouseReleased(ofMouseEventArgs &args)
{
    if( args.x > xPos - radius && args.x < xPos + radius &&
            args.y > yPos - radius && args.y < yPos + radius )
    {
        changeDept(dept);
        changeViewToOverview();
    }
}

void RoundButton::changeDept(std::string d)
{
    static CustomEvent newEvent;
    newEvent.dept = dept;
    ofNotifyEvent(CustomEvent::deptChange, newEvent);
}

void RoundButton::changeViewToOverview()
{
    static CustomEvent changeViewEvent;
    changeViewEvent.view = OVERVIEW;
    ofNotifyEvent(CustomEvent::viewChange, changeViewEvent);
}

bool RoundButton::hoverState(int x, int y)
{
    if( x > xPos - radius && x < xPos + radius &&
            y > yPos - radius && y < yPos + radius )
    {
        hoovering = true;
        actionTeaTimer++;
        if(actionTeaTimer > actionTeaDelay)
        {
            actionTeaTimer = 0;
            return true;
        }
        return false;
    }

    hoovering = false;
    actionTeaTimer = 0;
    return false;
}
