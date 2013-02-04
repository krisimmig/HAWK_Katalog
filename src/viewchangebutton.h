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
    viewsEnum targetView;

    void draw(ofEventArgs &e);
    void changeView(viewsEnum target);
    bool hoverState(int x, int y);
    void mouseReleased(ofMouseEventArgs &args);
    void mouseMoved(ofMouseEventArgs &args);

    bool active;

};

#endif // VIEWCHANGEBUTTON_H
