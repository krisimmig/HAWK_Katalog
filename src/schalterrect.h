#ifndef SCHALTERRECT_H
#define SCHALTERRECT_H

#include "ofMain.h"
#include "constants.h"

class schalterRect
{
public:
    schalterRect();
    void setup(string _label);

    int xPos;
    int getXPos();
    int yPos;
    int getYPos();
    int radius;
    string label;

    int actionTimer;
    int actionDelay;

    void draw(int x, int y);
    bool hoverState(int x, int y);
    bool mouseReleased(int x, int y, int button);

protected:
private:
};

#endif // SCHALTERRECT_H
