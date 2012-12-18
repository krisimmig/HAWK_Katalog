#ifndef BASEBUTTON_H
#define BASEBUTTON_H

#include "ofMain.h"
#include "constants.h"
#include "customevent.h"

class BaseButton
{
public:

    BaseButton();
    ~BaseButton();
    virtual void addListeners();
    virtual void removeListeners();

    int xPos;
    int yPos;
    int radius;
    string label;


    int actionTimer;
    int actionDelay;

    void _draw(ofEventArgs &e);
    virtual void draw();


protected:
private:
};

#endif // BASEBUTTON_H
