#include "testApp.h"
#include "stdio.h"

testApp::testApp()
{

}

void testApp::setup()
{
    ofEnableSmoothing();
    ofSetFrameRate(30);
    float filterFactor = 0.1f;

    garamondRegularH1.loadFont("fonts/AGaramondPro-Regular.otf", 25);

    // use app with or without kinect
    useKinect = true;
    if(useKinect)
    {
        // ---------------------------------
        // kinect setup
        // ---------------------------------
        openNIDevice.setup();
        openNIDevice.addDepthGenerator();
        openNIDevice.setMirror(true);
        openNIDevice.addUserGenerator();
        openNIDevice.addHandsGenerator();
        openNIDevice.addAllHandFocusGestures();
        openNIDevice.setMaxNumUsers(3);
        openNIDevice.setMaxNumHands(2);
        openNIDevice.start();
    }

    viewmanager.setCursor(cursor);
    cursorRadius = 40;

}

void testApp::update()
{
    if(useKinect)
    {
        openNIDevice.update();
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

            // only PC, see http://forum.openframeworks.cc/index.php/topic,1438.0.html

        }

        // upadte cursor position
        cursor.update(cursorXPos, cursorYPos);
        // get number of current hands
        int numUsers = openNIDevice.getNumTrackedUsers();

        userInfo = ofToString(numUsers);
        if(numUsers > 0)
        {
            for (int i = 0; i < numUsers; i++)
            {
                user = &openNIDevice.getTrackedUser(i);
                userInfo += ", id: ";
                userInfo += ofToString(user->getXnID());
                user->drawSkeleton();

                // rotation
                float leftHandZ = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().z;
                float leftShoulderZ = user->getJoint(JOINT_LEFT_SHOULDER).getWorldPosition().z;
                if( (leftShoulderZ - leftHandZ) > 400 )
                {
                    // left - right
                    float leftElbowX = user->getJoint(JOINT_LEFT_ELBOW).getWorldPosition().x;
                    float leftHandX = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().x;
                    kinectMovementX = leftElbowX - leftHandX;

                    // up - down
                    float elbowY = user->getJoint(JOINT_LEFT_ELBOW).getWorldPosition().y;
                    float handY = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().y;
                    kinectMovementY = elbowY - handY;
                }
                else
                {
                    kinectMovementX = 0;
                    kinectMovementY = 0;
                }

                // speed

                float rightHandZ = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().z;
                float rightShoulderZ = user->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;
                if( (rightShoulderZ - rightHandZ) > 200 || (rightShoulderZ - rightHandZ) < -200  )
                {

                    kinectMovementZ = rightShoulderZ - rightHandZ;
                }
                else
                {
                    kinectMovementZ = 0;
                }
            }
        }
        cursor.kinectMovement.set(kinectMovementX, kinectMovementY, kinectMovementZ);
    }
}

void testApp::draw()
{

//    ofSetColor(0,0,0);
//    garamondRegularH1.drawString("User: " + userInfo, 550, 45);

    if(trackingHand)
    {
        SetCursorPos(cursorXPos + cursorRadius/2, cursorYPos + cursorRadius/2);
    }
    if (cursor.visible)
    {
        ofShowCursor();
    }
    else
    {
        ofHideCursor();
    }
}

void testApp::exit()
{
    if(useKinect)
    {
        openNIDevice.stop();
    }
}
