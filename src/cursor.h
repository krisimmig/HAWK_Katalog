#ifndef CURSOR_H
#define CURSOR_H

#include "ofMain.h"

class HandCursor
{
public:
    HandCursor();
    void setup(int x, int y);
    void update(int x, int y);
    void draw();
    int xPos;
    int yPos;
    bool cursorDrag;
    ofVec2f moveVector;

};

#endif // CURSOR_H
