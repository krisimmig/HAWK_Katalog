#include "button.h"

RoundButton::RoundButton()
{
    //ctor
}

void RoundButton::setup(string _label)
{
    label = _label;
    radius = 40;
    actionTimer = 0;
    actionDelay = 30;
}

void RoundButton::draw(int x, int y)
{
    xPos = x;
    yPos = y;
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofCircle(xPos, yPos, radius);
    ofDrawBitmapString(label, xPos - 20, yPos + 5);
}

bool RoundButton::hoverState(int x, int y)
{
    if( x > xPos - radius && x < xPos + radius &&
            y > yPos - radius && y < yPos + radius )
    {

        // count down graphic
        ofPath timerShape;
        timerShape.setFilled(false);
        timerShape.setStrokeWidth(10);
        timerShape.setStrokeColor(ofColor(255, 0, 0));
        timerShape.arc(xPos, yPos, radius, radius, 0, (360/actionDelay) * actionTimer);
        timerShape.draw();

        if(actionTimer > actionDelay)
        {
            actionTimer = 0;
            return true;
        }
        actionTimer++;
        return false;
    }
    actionTimer = 0;
    return false;
}

bool RoundButton::mouseReleased(int _x, int _y, int button)
{
    if(_x > xPos - radius && _x < xPos + radius &&
            _y > yPos - radius && _y < yPos + radius )
    {
        return true;
    }
    return false;
}
