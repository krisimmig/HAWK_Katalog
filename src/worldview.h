#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include "ofMain.h"
#include "constants.h"
#include "cursor.h"
#include "object3d.h"
#include "students.h"
#include "viewchangebutton.h"
#include "exitbutton.h"
#include "customevent.h"
#include "ofxassimpmodelloader.h"
#include "mycamera.h"



class WorldView
{
public:
    WorldView();
    virtual ~WorldView();
    void setCursor(HandCursor *cursor);
    void nearObjectListener(CustomEvent &e);

    bool drawInfo;

protected:
private:
    void draw(ofEventArgs &e);
    void update(ofEventArgs &e);
    void drawSphereInfo();
    void keyPressed(ofKeyEventArgs &e);
    ExitButton exitInfoScreenButton;

    HandCursor* cursor;

    // environment + camera
    ofxAssimpModelLoader model;
    ofLight light;
    MyCamera camera;
    float speed;
    float dollyAmount;

    // 3d objects
    Object3D** mySphere;
    int currentSphere;
    int sphereSize;

    // student stuff
    Students currentStudent;
    int numberOfStudents;
    int* studentIdArray;
    ofTrueTypeFont garamondRegularH1;
    ofTrueTypeFont garamondRegularS;
    ofImage projectImage01;

};

#endif // WORLDVIEW_H

