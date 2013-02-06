#include "cursor.h"

HandCursor::HandCursor()
{

}

void HandCursor::setup(float x, float y)
{
    xPos = x;
    yPos = y;

    cursorDrag = false;
    isActiveUser = false;
    trackingHand = false;
    trackingHand = false;

    activeUserId = -1;
    listSize = 10;

}

void HandCursor::update(float x, float y)
{
    xPos = x;
    yPos = y;

    calculatePos();
    calculateActiveUserMiddlePosX();

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

void HandCursor::calculateActiveUserMiddlePosX()
{
    // x
    listOfXPositionActiveUser.push_front(activeUserXpos);
    if(listOfXPositionActiveUser.size() > listSize)
    {
        listOfXPositionActiveUser.pop_back();
    }

    float listSumX = 0.0f;
    for(std::list<float>::iterator list_iter = listOfXPositionActiveUser.begin(); list_iter != listOfXPositionActiveUser.end(); list_iter++)
    {
        listSumX += *list_iter;
    }

    smoothActiveUserXPos = listSumX / listOfXPositionActiveUser.size();
}
void HandCursor::calculatePos()
{
    // x
    listOfXPositions.push_front(xPos);
    if(listOfXPositions.size() > listSize)
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
    if(listOfYPositions.size() > listSize)
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

    if(listOfLeftXPositions.size() > listSize)
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
    if(listOfLeftYPositions.size() > listSize)
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
