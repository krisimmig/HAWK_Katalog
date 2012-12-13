#ifndef CURSOR_H
#define CURSOR_H

#include "ofMain.h"

class HandCursor
{
public:
    HandCursor();
    void setup(int x, int y, int r);

    int radius;
    int xPos;
    int yPos;


    void update(int x, int y);
    void draw();

protected:
private:
};

#endif // CURSOR_H
