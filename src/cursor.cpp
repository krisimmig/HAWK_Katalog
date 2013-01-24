#include "cursor.h"

HandCursor::HandCursor()
{

}

void HandCursor::setup(int x, int y)
{
    xPos = x;
    yPos = y;
    cursorDrag = false;
    moveVector.set(x,y);
}

void HandCursor::update(int x, int y)
{
    xPos = x;
    yPos = y;
    moveVector.set(x,y);
}

void HandCursor::draw()
{

}
