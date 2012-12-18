#include "view3d.h"

View3D::View3D()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &View3D::draw);
//    buttonToOverview = new ViewChangeButton("Overview", OVERVIEW, 600, 150);
    cout << "*** View3D created." << endl;
}

View3D::~View3D()
{
    //dtor
    ofRemoveListener(ofEvents().draw, this, &View3D::draw);
//    delete buttonToOverview;
    cout << "*** View3D deleted." << endl;
}

void View3D::draw(ofEventArgs &e)
{
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("3D View", ofGetWidth() / 2, 50);
}
