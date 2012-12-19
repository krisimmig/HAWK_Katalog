#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include "ofMain.h"
#include "constants.h"
#include "cursor.h"
#include "object3d.h"
#include "viewchangebutton.h"
#include "customevent.h"
#include "ofxassimpmodelloader.h"
#include "mycamera.h"



class WorldView
{
public:
    WorldView();
    virtual ~WorldView();
    void setCursor(HandCursor *cursor);

protected:
private:
    void draw(ofEventArgs &e);
    void update(ofEventArgs &e);
    void keyPressed(ofKeyEventArgs &e);
    ViewChangeButton* buttonToMainview;

    float speed;
    float dollyAmount;
    ofTrueTypeFont garamondRegularH1;
    std::string sphereInfo;

    MyCamera camera;
    ofLight light;
    HandCursor* cursor;
    Object3D mySphere[50];
    ofxAssimpModelLoader model;
};

#endif // WORLDVIEW_H
