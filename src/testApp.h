#ifndef _TEST_APP
#define _TEST_APP

#define MAX_DEVICES 2

#include "constants.h"
#include "ofMain.h"
#include "cursor.h"

#include "ofxOpenNI.h"
#include "viewmanager.h"
#include "mainmenu.h"
#include "overview.h"
#include "singleview.h"

class testApp : public ofBaseApp
{
public:
    testApp();
    void setup();
    void update();
    void draw();
    void exit();

    void changeNotifications();

    // views
    ViewManager viewmanager;
    MainMenu m_view;
    Overview o_view;
    Singleview s_view;
    enum_views currentView;

    // cursor
    ofxOpenNI openNIDevice;
    bool useKinect;
    HandCursor cursor;
    int cursorXPos;
    int cursorYPos;
    bool trackingHand;
    int actionDelay;
    int cursorRadius;

};

#endif
