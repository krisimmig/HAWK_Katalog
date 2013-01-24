#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include "ofMain.h"
#include "constants.h"
#include "cursor.h"
#include "object3d.h"
#include "customevent.h"



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
    void keyReleased(ofKeyEventArgs &e);
    void keyPressed(ofKeyEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void changeZoomLevel(int zoomLevel);
    void zoomChangeListener(CustomEvent &e);
    void moveScreen(ofVec2f moveVector);
    void kinectMove();
    HandCursor* cursor;

    // environment + camera
    ofCamera camera;
    ofLight light;
    float cameraHeight;
    float dragSpeed;
    float currentDragSpeed;
    int zoomLevel;

    float pmouseX;
    float pmouseY;

    // 3d objects
    Object3D** mySphere;
    int currentSphere;
    int sphereSize;
    int gestureTimer;

    // student stuff
    int numberOfStudents;
//    int* studentIdArray;
    ofTrueTypeFont Helvetica22;
    ofTrueTypeFont Helvetica15;


    // fog
    float fogDistance;
};

#endif // WORLDVIEW_H

