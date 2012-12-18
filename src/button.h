#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"
#include "constants.h"
#include "customevent.h"

class RoundButton
{
public:

    RoundButton(std::string label, std::string dept, int x, int y);
    virtual ~RoundButton();

protected:
private:
    void addListeners();
    int xPos;
    int yPos;
    int radius;
    string label;
    string dept;

    int actionTeaTimer;
    int actionTeaDelay;

    void draw(ofEventArgs &e);
    void mouseReleased(ofMouseEventArgs &args);
    void mouseMoved(ofMouseEventArgs &args);
    void changeDept(std::string d);
    void changeViewToOverview();
    bool hoverState(int x, int y);
    bool hoovering;
};

#endif // BUTTON_H
