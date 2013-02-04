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
    void emptyLists();
    void calculateLeftPos();
    void draw();

    float usersPos[6];

    float xPos;
    float yPos;

    float leftXPos;
    float leftYPos;

    float smoothRightXPos;
    float smoothRightYPos;

    std::list<float> listOfXPositions;
    std::list<float> listOfYPositions;

    int activeUserId;
    float activeUserPos;

    float smoothLeftXPos;
    float smoothLeftYPos;

    std::list<float> listOfLeftXPositions;
    std::list<float> listOfLeftYPositions;

    bool cursorDrag;
    ofVec2f moveVector;
    ofVec2f leftVector;

    bool isActiveUser;
    bool trackingHand;

    // gestures
    float zoomOutGestureTimer;
    float zoomInGestureTimer;
    int zoomGestureDuration;


};

#endif // CURSOR_H
