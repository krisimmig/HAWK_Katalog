#ifndef VIEWCHANGEBUTTON_H
#define VIEWCHANGEBUTTON_H

#include "ofMain.h"
#include "constants.h"
#include "customevent.h"

class ViewChangeButton
{
public:
    ViewChangeButton(std::string label, enum_views targetView, int x, int y);
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
    enum_views targetView;

    void draw(ofEventArgs &e);
    void changeView(enum_views target);
    bool hoverState(int x, int y);
    void mouseReleased(ofMouseEventArgs &args);
    void mouseMoved(ofMouseEventArgs &args);

};

#endif // VIEWCHANGEBUTTON_H
