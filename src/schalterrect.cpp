#include "schalterrect.h"

schalterRect::schalterRect()
{
    //ctor
}

void schalterRect::setup(string _label)
{
    //ctor
    label = _label;
    radius = 40;
    actionTimer = 0;
    actionDelay = 30;
}


int schalterRect::getXPos()
{
    return xPos;
}

int schalterRect::getYPos()
{
    return yPos;
}

void schalterRect::draw(int x, int y)
{
    xPos = x;
    yPos = y;
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofCircle(xPos, yPos, radius);
    ofDrawBitmapString(label, xPos - 20, yPos + 5);
}

bool schalterRect::hoverState(int x, int y)
{
    if( x > xPos - radius && x < xPos + radius &&
            y > yPos - radius && y < yPos + radius )
    {
        ofPath timerShape;
        timerShape.setFilled(false);
        timerShape.setStrokeWidth(10);
        timerShape.setStrokeColor(ofColor(255, 0, 0));
//        timerShape.moveTo(x, y);
        timerShape.arc(x, y, 40, 40, 0, (360/actionDelay) * actionTimer);
        timerShape.draw();

        if(actionTimer > actionDelay)
        {
            actionTimer = 0;
            return true;
        }
        cout << "-- CURSOR TIMER: " << actionTimer << endl;
        actionTimer++;
        return false;
    }
    actionTimer = 0;
    return false;
}

bool schalterRect::mouseReleased(int _x, int _y, int button)
{

    if(_x > xPos - radius && _x < xPos + radius &&
            _y > yPos - radius && _y < yPos + radius )
    {
        return true;
    }
    return false;
}
