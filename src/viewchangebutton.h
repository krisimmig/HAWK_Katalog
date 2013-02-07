#ifndef VIEWCHANGEBUTTON_H
#define VIEWCHANGEBUTTON_H

#include "ofMain.h"
#include "constants.h"
#include "customevent.h"

class ViewChangeButton
{
public:
    ViewChangeButton(std::string label, viewsEnum targetView, int x, int y);
    virtual ~ViewChangeButton();
    viewsEnum targetView;

    void changeView(viewsEnum target);

protected:
private:
    void addListeners();
    int xPos;
    int yPos;
    int width;
    int height;
    int actionDelay;
    int actionTimer;
    bool hoovering;
    string label;


    void draw(ofEventArgs &e);
    bool hoverState(int x, int y);
    void mouseReleased(ofMouseEventArgs &args);
    void mouseMoved(ofMouseEventArgs &args);

    bool active;

};

#endif // VIEWCHANGEBUTTON_H
