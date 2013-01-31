#include "cursor.h"

HandCursor::HandCursor()
{

}

void HandCursor::setup(float x, float y)
{
    xPos = x;
    yPos = y;

    cursorDrag = false;
    calibratingUser = false;
    trackingHand = false;
    trackingHand = false;
}

void HandCursor::update(float x, float y)
{
    xPos = x;
    yPos = y;

    calculatePos();

    moveVector.set(smoothRightXPos,smoothRightYPos);
}

void HandCursor::updateLeftHanded(float x, float y)
{
    leftXPos = x;
    leftYPos = y;

    calculateLeftPos();

    leftVector.set(smoothLeftXPos,smoothLeftYPos);
}

void HandCursor::draw()
{

}

void HandCursor::emptyLists()
{
    listOfLeftXPositions.clear();
    listOfLeftYPositions.clear();

    float listSum = 0.0f;
    for(std::list<float>::iterator list_iter = listOfLeftYPositions.begin(); list_iter != listOfLeftYPositions.end(); list_iter++)
    {
        listSum += *list_iter;
    }
    float sumY = listSum / listOfLeftYPositions.size();

    listSum = 0.0f;
    for(std::list<float>::iterator list_iter = listOfLeftXPositions.begin(); list_iter != listOfLeftXPositions.end(); list_iter++)
    {
        listSum += *list_iter;
    }
    float sumX = listSum / listOfLeftXPositions.size();
}

void HandCursor::calculatePos()
{
    // x
    listOfXPositions.push_front(xPos);
    if(listOfXPositions.size() > 15)
    {
        listOfXPositions.pop_back();
    }

    float listSumX = 0.0f;
    for(std::list<float>::iterator list_iter = listOfXPositions.begin(); list_iter != listOfXPositions.end(); list_iter++)
    {
        listSumX += *list_iter;
    }

    smoothRightXPos = listSumX / listOfXPositions.size();

    // y
    listOfYPositions.push_front(yPos);
    if(listOfYPositions.size() > 15)
    {
        listOfYPositions.pop_back();
    }

    float listSumY = 0.0f;
    for(std::list<float>::iterator list_iter = listOfYPositions.begin(); list_iter != listOfYPositions.end(); list_iter++)
    {
        listSumY += *list_iter;
    }

    smoothRightYPos = listSumY / listOfYPositions.size();
}

void HandCursor::calculateLeftPos()
{
    // x
    listOfLeftXPositions.push_front(leftXPos);

    if(listOfLeftXPositions.size() > 15)
    {
        listOfLeftXPositions.pop_back();
    }

    float listSumX = 0.0f;
    for(std::list<float>::iterator list_iter = listOfLeftXPositions.begin(); list_iter != listOfLeftXPositions.end(); list_iter++)
    {
        listSumX += *list_iter;
    }

    smoothLeftXPos = listSumX / listOfLeftXPositions.size();

    // y
    listOfLeftYPositions.push_front(leftYPos);
    if(listOfLeftYPositions.size() > 15)
    {
        listOfLeftYPositions.pop_back();
    }

    float listSumY = 0.0f;
    for(std::list<float>::iterator list_iter = listOfLeftYPositions.begin(); list_iter != listOfLeftYPositions.end(); list_iter++)
    {
        listSumY += *list_iter;
    }
    smoothLeftYPos = listSumY / listOfLeftYPositions.size();
}
