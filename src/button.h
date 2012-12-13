#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"
#include "constants.h"

class RoundButton
{
public:
    RoundButton();
    void setup(string _label);

    int xPos;
    int yPos;
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

#endif // BUTTON_H
