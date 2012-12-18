#ifndef VIEW3D_H
#define VIEW3D_H

#include "ofMain.h"
#include "constants.h"
#include "viewchangebutton.h"
#include "customevent.h"

class View3D
{

public:
    View3D();
    ~View3D();
protected:
private:
    void draw(ofEventArgs &e);
//    ViewChangeButton* buttonToOverview;

};


#endif // VIEW3D_H
