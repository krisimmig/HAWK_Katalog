#include "exitbutton.h"

ExitButton::ExitButton()
{

}

ExitButton::~ExitButton()
{
    ofRemoveListener(ofEvents().draw, this, &ExitButton::draw);
    ofRemoveListener(ofEvents().mouseReleased, this, &ExitButton::mouseReleased);
    ofRemoveListener(ofEvents().mouseMoved, this, &ExitButton::mouseMoved);
}

void ExitButton::setup(std::string l, int x, int y)
{
    //ctor
    label = l;
    xPos = x;
    yPos = y;

    radius = 40;
    actionTeaDelay = 25;
    actionTeaTimer = 0;
    hoovering = false;
    active = false;
    addListeners();

}


//add listener stuff
void ExitButton::addListeners()
{
    //app events
    ofAddListener(ofEvents().draw, this, &ExitButton::draw);
    ofAddListener(ofEvents().mouseReleased, this, &ExitButton::mouseReleased);
    ofAddListener(ofEvents().mouseMoved, this, &ExitButton::mouseMoved);
}

void ExitButton::draw(ofEventArgs &e)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    if(active)
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void ExitButton::mouseMoved(ofMouseEventArgs &args)
{
    if(hoverState(args.x, args.y) )
    {
        active = false;
    }
}

void ExitButton::mouseReleased(ofMouseEventArgs &args)
{
    if( args.x > xPos - radius && args.x < xPos + radius &&
            args.y > yPos - radius && args.y < yPos + radius )
    {
        active = false;
    }
}

bool ExitButton::hoverState(int x, int y)
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
