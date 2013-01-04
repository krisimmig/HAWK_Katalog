#ifndef EXITBUTTON_H
#define EXITBUTTON_H

#include "ofMain.h"
#include "constants.h"
#include "customevent.h"

class ExitButton
{
public:

    ExitButton();
    void setup(std::string label, int x, int y);
    virtual ~ExitButton();
    bool active;

protected:
private:
    void addListeners();
    int xPos;
    int yPos;
    int radius;
    string label;

    int actionTeaTimer;
    int actionTeaDelay;

    void draw(ofEventArgs &e);
    void mouseReleased(ofMouseEventArgs &args);
    void mouseMoved(ofMouseEventArgs &args);

    bool hoverState(int x, int y);
    bool hoovering;

};

#endif // EXITBUTTON_H
