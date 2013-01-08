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
    void drawLandscape();
    void drawScore();
    void keyPressed(ofKeyEventArgs &e);
    ExitButton exitInfoScreenButton;

    HandCursor* cursor;

    // environment + camera
    GLfloat environmentColor[3];
    ofxAssimpModelLoader landscape;
    ofLight light;
    MyCamera camera;
    float speed;
    float dollyAmount;

    // 3d objects
    ofxAssimpModelLoader human;
    float rotateDegree;
    Object3D** mySphere;
    int currentSphere;
    int sphereSize;
    int avatarXPos;
    int avatarYPos;
    int avatarZPos;
    ofVec3f avatarPos;

    // student stuff
    Students currentStudent;
    int numberOfStudents;
    int* studentIdArray;
    ofTrueTypeFont Helvetica22;
    ofTrueTypeFont Helvetica15;
    ofImage infoBackground;

    // fog
    float fogDistance;
    int score;

};

#endif // WORLDVIEW_H

