#ifndef CURSOR_H
#define CURSOR_H

#include "ofMain.h"

class HandCursor
{
public:
    HandCursor();
    void setup(float x, float y);
    void update(float x, float y);
    void updateLeftHanded(float x, float y);
    void calculatePos();
    void calculateLeftPos();
    void calculateActiveUserMiddlePosX();
    void emptyLists();
    void draw();

    float usersPos[6];

    float xPos;
    float yPos;
    float leftXPos;
    float leftYPos;
    float activeUserXpos;
    float activeUserPos;
    int activeUserId;

    float smoothRightXPos;
    float smoothRightYPos;
    float smoothActiveUserXPos;
    float smoothLeftXPos;
    float smoothLeftYPos;

    std::list<float> listOfXPositions;
    std::list<float> listOfYPositions;
    std::list<float> listOfLeftXPositions;
    std::list<float> listOfLeftYPositions;
    std::list<float> listOfXPositionActiveUser;
    int listSize;


    ofVec2f moveVector;
    ofVec2f leftVector;

    bool isActiveUser;
    bool trackingHand;
    bool twoHands;
    bool cursorDrag;

    // gestures
    float zoomOutGestureTimer;
    float zoomInGestureTimer;
    int zoomGestureDuration;


};

#endif // CURSOR_H
