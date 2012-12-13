#include "cursor.h"

HandCursor::HandCursor()
{

}

void HandCursor::setup(int x, int y, int r)
{
    xPos = x;
    yPos = y;
    radius = r;
}

void HandCursor::update(int x, int y)
{
    xPos = x;
    yPos = y;
}

void HandCursor::draw()
{
    ofEnableAlphaBlending();
    ofSetColor(20,20,20, 127);
    ofCircle(xPos, yPos, radius);
    ofDisableAlphaBlending();
}
