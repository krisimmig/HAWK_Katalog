#ifndef CURSOR_H
#define CURSOR_H

#include "ofMain.h"

class HandCursor
{
public:
    HandCursor();
    void setup(int x, int y, int r);
    void update(int x, int y);
    void draw();
    int xPos;
    int yPos;

    ofVec3f kinectMovement;
    bool visible;

protected:
private:
    int radius;

};

#endif // CURSOR_H
