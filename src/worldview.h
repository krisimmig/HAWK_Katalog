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
    void drawInfo();
    void drawSucher();
    void drawBottomInterface();

    void update(ofEventArgs &e);
    void updateZoomLevel();
    void updateScreenPosition();

    void keyReleased(ofKeyEventArgs &e);
    void keyPressed(ofKeyEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void changeZoomLevel(zoomLevelEnum zoomLevel);
    void swipeGestureEvent(swipeGesturesEnum swipeDirection);
    void zoomChangeListener(CustomEvent &e);
    void swipeGestureListener(CustomEvent &e);
    void moveScreen(ofVec2f moveVector);
    void kinectMove();
    string wrapString(string text, int width);
    HandCursor* cursor;

    // environment + camera
    ofCamera camera;
    float cameraHeight;
    float currentCameraHeight;
    float dragSpeed;
    float currentXDragSpeed;
    float currentYDragSpeed;
    int zoomLevel;
    bool zooming;
    int random10;
    int speedXCounter;
    int speedYCounter;
    float speedFactor;

    float pmouseX;
    float pmouseY;

    float cameraX;
    float cameraY;

    // 3d objects
    Object3D** mySphere;
    int currentSphere;
    int sphereSize;
    int gestureTimerZoom;
    int gestureTimerSwipe;
    int gestureTimeout;
    ofVec3f closestObjectVector;

    // student stuff
    int numberOfStudents;
    int currentStudent;
    int projectImagesYPosition;
    int currentProjectImagesYPosition;
    int currentImageNumber;
    int currentImageHeight;
    std::string fullName;
    std::string description;
//    int* studentIdArray;
    ofTrueTypeFont HelveticaXL;
    ofTrueTypeFont HelveticaL;
    ofTrueTypeFont HelveticaS;

};

#endif // WORLDVIEW_H

