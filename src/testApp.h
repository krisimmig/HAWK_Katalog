#ifndef _TEST_APP
#define _TEST_APP

#define MAX_DEVICES 2

#include "constants.h"
#include "ofMain.h"
#include "cursor.h"

#include "ofxOpenNI.h"
#include "viewmanager.h"

class testApp : public ofBaseApp
{
public:
    testApp();
    void setup();
    void update();
    void draw();
    void exit();
    void changeZoomLevel(int _zoomLevel);
    float smoothValue(float value);

    ofTrueTypeFont garamondRegularH1;

    // views
    ViewManager viewmanager;

    // cursor
    ofxOpenNI openNIDevice;
    ofxOpenNIUser* user;
    bool useKinect;
    HandCursor cursor;
    float cursorXPos;
    float cursorYPos;

    float leftHandX;
    float leftHandY;

    // gestures
    int zoomOutGestureTimer;
    int zoomInGestureTimer;
    int gestureDuration;
    int  minZoomGestureDistance;

    float kinectMovementX;
    float kinectMovementY;
    float kinectMovementZ;
    float handsDistance;
    float previousHandsDistance;

    bool twoHands;
    std::string userInfo;

    int userId;

};

#endif
