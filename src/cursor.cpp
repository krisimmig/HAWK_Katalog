#include "cursor.h"

HandCursor::HandCursor()
{
    visible = true;
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

}
