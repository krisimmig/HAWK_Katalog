#ifndef OVERVIEW_H
#define OVERVIEW_H

#include "ofMain.h"
#include "view.h"
#include "customevent.h"
#include "button.h"

class Overview : public View
{
public:
    Overview();
    virtual void setupButtons();
    void draw();
    void deptChanged(CustomEvent &args);

    std::string fachbereich;
    RoundButton singleviewButton;
    RoundButton mainmenuButton;
    void myMouseReleased(ofMouseEventArgs &args);

protected:
private:
};

#endif // OVERVIEW_H
