#include "testApp.h"
#include "stdio.h"


testApp::testApp()
{

}

void testApp::setup()
{
    ofBackground(255, 255, 255);
    ofSetColor(50,20,230);

    ofEnableSmoothing();
    ofSetFrameRate(30);

    // use app with or without kinect
    useKinect = true;
    if(useKinect)
    {
        // ---------------------------------
        // kinect setup
        // ---------------------------------
        openNIDevice.setup();
        openNIDevice.addDepthGenerator();
        openNIDevice.setRegister(true);
        openNIDevice.setMirror(true);
        // setup the hand generator
        openNIDevice.addHandsGenerator();
        // add all focus gestures (ie., wave, click, raise arm)
        openNIDevice.addAllHandFocusGestures();
        openNIDevice.setMaxNumHands(4);
        openNIDevice.start();
    }

    // ---------------------------------
    // cursor setup
    // ---------------------------------
    cursorXPos = -50;
    cursorYPos = -50;
    cursorRadius = 30;
    cursor.setup(cursorXPos, cursorYPos, cursorRadius);

    // ---------------------------------
    // views setup
    // ---------------------------------
    o_view.setup(cursor, viewmanager);
    m_view.setup(cursor, viewmanager);
    s_view.setup(cursor, viewmanager);
    changeNotifications();
}

void testApp::changeNotifications()
{
    currentView = viewmanager.getCurrentView();
    switch (currentView)
    {
    case MAINMENU :
        m_view.update(trackingHand);
        ofAddListener(ofEvents().mouseReleased, &m_view, &MainMenu::myMouseReleased);
        ofRemoveListener(ofEvents().mouseReleased, &s_view, &Singleview::myMouseReleased);
        ofRemoveListener(ofEvents().mouseReleased, &o_view, &Overview::myMouseReleased);
        break;
    case OVERVIEW :
        o_view.update(trackingHand);
        ofAddListener(ofEvents().mouseReleased, &o_view, &Overview::myMouseReleased);
        ofRemoveListener(ofEvents().mouseReleased, &s_view, &Singleview::myMouseReleased);
        ofRemoveListener(ofEvents().mouseReleased, &m_view, &MainMenu::myMouseReleased);
        break;
    case SINGLE :
        s_view.update(trackingHand);
        ofAddListener(ofEvents().mouseReleased, &s_view, &Singleview::myMouseReleased);
        ofRemoveListener(ofEvents().mouseReleased, &m_view, &MainMenu::myMouseReleased);
        ofRemoveListener(ofEvents().mouseReleased, &o_view, &Overview::myMouseReleased);
        break;
    }
}
void testApp::update()
{
    ofSetBackgroundColor(245, 255, 250);
    ofSetColor(10,0,10);

    if(useKinect)
    {
        openNIDevice.update();
        // upadte cursor position
        cursor.update(cursorXPos, cursorYPos);
    }

    // update active view
    if(currentView != viewmanager.getCurrentView())
    {
        changeNotifications();
    }

    switch (currentView)
    {
    case MAINMENU :
        m_view.update(trackingHand);
        break;
    case OVERVIEW :
        o_view.update(trackingHand);
        break;
    case SINGLE :
        s_view.update(trackingHand);
        break;
    }

}

void testApp::draw()
{

    // get number of current hands
    if(useKinect)
    {
        int numHands = openNIDevice.getNumTrackedHands();

        // when no hand is tracked, dissappear handCurosr
        if(numHands == 0)
        {
            trackingHand = false;
            cursorXPos = -50;
        }
        // iterate through users
        for (int i = 0; i < numHands; i++)
        {
            // set tracking user to true
            trackingHand = true;

            // get a reference to this user
            ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
            ofPoint & handPosition = hand.getPosition();

            // set cursor Position & adjust to screensize
            cursorXPos = ( handPosition.x / (600 / 100) ) * (ofGetWidth() / 100);
            cursorYPos = ( handPosition.y / (440 / 100) ) * (ofGetHeight() / 100);
        }
    }

    // ---
    // view switcher
    // ---
    switch (currentView)
    {
    case MAINMENU :
        m_view.draw();
        break;
    case OVERVIEW :
        o_view.draw();
        break;
    case SINGLE :
        s_view.draw();
        break;
    }
    // draw cursor last / always ontop
    cursor.draw();
}

void testApp::exit()
{
    if(useKinect)
    {
        openNIDevice.stop();
    }
}

