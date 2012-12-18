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

    ofTrueTypeFont garamondRegularH1;

    // views
//    WorldView w_view;
    ViewManager viewmanager;

    // cursor
    ofxOpenNI openNIDevice;
    ofxOpenNIUser* user;
    bool useKinect;
    HandCursor cursor;
    int cursorXPos;
    int cursorYPos;
    float kinectMovementX;
    float kinectMovementY;
    float kinectMovementZ;
    float radius;
    bool trackingHand;
    std::string userInfo;

};

#endif
