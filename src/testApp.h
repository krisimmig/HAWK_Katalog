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
    void changeZoomLevel(zoomLevelEnum _zoomLevel);
    void kinectGestures();
    float smoothValue(float value);

    ofTrueTypeFont garamondRegularH1;

    // views
    ViewManager viewmanager;

    // kinect - user
    ofxOpenNI openNIDevice;
    ofxOpenNIUser* user;
    ofxOpenNIUser* activeUser;
    bool isActiveUser;
    bool useKinect;
    std::string userInfo;
    int activeUserId;
    int numberOfMaxUsers;

    // debug messages
    bool debugGestures;
    bool debugUsers;

    // cursor
    HandCursor cursor;
    float cursorXPos;
    float cursorYPos;

    float leftHandX;
    float leftHandY;

    // gestures
    int zoomOutGestureTimer;
    int zoomInGestureTimer;
    int zoomGestureDuration;
    int swipeTimer;
    int swipeLeftTimer;
    int swipeRightTimer;
    int swipeUpTimer;
    int swipeDownTimer;
    int swipeTimerDefaultX;
    int swipeTimerDefaultY;
    int swipeTimerDefaultDOWN;
    int minZoomGestureDistance;
    int zoomInTimer;
    int zoomOutTimer;
    int zoomTimerDefault;
    bool swipingLeft;
    bool swipingRight;
    bool swipingUp;
    bool swipingDown;
    bool zoomIn;
    bool zoomOut;

    float kinectMovementX;
    float kinectMovementY;
    float kinectMovementZ;
    float handsDistance;
    float previousHandsDistance;
    float previousRightX;
    float previousRightY;

    bool twoHands;


};

#endif
