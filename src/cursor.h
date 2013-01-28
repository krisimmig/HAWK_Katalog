#ifndef CURSOR_H
#define CURSOR_H

#include "ofMain.h"

class HandCursor
{
public:
    HandCursor();
    void setup(int x, int y);
    void update(int x, int y);
    void calculatePos();
    void draw();
    bool coordsReady();
    int xPos;
    int yPos;
    float smoothXPos;
    float smoothYPos;
    bool cursorDrag;
    ofVec2f moveVector;
    std::list<float> listOfXPositions;
    std::list<float> listOfYPositions;

    bool calibratingUser;
    bool trackingUser;
    bool trackingHand;



};

#endif // CURSOR_H
