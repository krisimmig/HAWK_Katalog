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
    int xPos;
    int yPos;
    int smoothXPos;
    int smoothYPos;
    bool cursorDrag;
    ofVec2f moveVector;
    std::list<int> listOfXPositions;
    std::list<int> listOfYPositions;

    bool calibratingUser;
    bool trackingUser;
    bool trackingHand;



};

#endif // CURSOR_H
