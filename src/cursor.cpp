#include "cursor.h"

HandCursor::HandCursor()
{

}

void HandCursor::setup(int x, int y)
{
    xPos = x;
    yPos = y;

    cursorDrag = false;
    calibratingUser = false;
    trackingHand = false;
    trackingHand = false;
}

void HandCursor::update(int x, int y)
{
    xPos = x;
    yPos = y;

    calculatePos();

    moveVector.set(smoothXPos,smoothYPos);
//    moveVector.set(xPos,yPos);
}

void HandCursor::draw()
{

}

void HandCursor::calculatePos()
{

    // if not moving, empty lists
    if(!cursorDrag)
    {
        listOfXPositions.empty();
        listOfYPositions.empty();
    }

    // x
    listOfXPositions.push_front(xPos);
    if(listOfXPositions.size() > 15)
    {
        listOfXPositions.pop_back();
    }

    float listSumX = 0.0f;
    for(std::list<int>::iterator list_iter = listOfXPositions.begin(); list_iter != listOfXPositions.end(); list_iter++)
    {
        listSumX += *list_iter;
    }

    smoothXPos = listSumX / listOfXPositions.size();

    // y
    listOfYPositions.push_front(yPos);
    if(listOfYPositions.size() > 15)
    {
        listOfYPositions.pop_back();
    }

    float listSumY = 0.0f;
    for(std::list<int>::iterator list_iter = listOfYPositions.begin(); list_iter != listOfYPositions.end(); list_iter++)
    {
        listSumY += *list_iter;
    }

    smoothYPos = listSumY / listOfYPositions.size();

}
