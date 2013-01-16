#include "testApp.h"
#include "stdio.h"

testApp::testApp()
{
    viewmanager.setCursor(cursor);
}

void testApp::setup()
{
    ofEnableSmoothing();
    ofSetFrameRate(30);
    float filterFactor = 0.1f;

    garamondRegularH1.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 25);

    // use app with or without kinect
    useKinect = false;
    if(useKinect)
    {
        // ---------------------------------
        // kinect setup
        // ---------------------------------
        openNIDevice.setup();
        openNIDevice.addDepthGenerator();
        openNIDevice.addImageGenerator();
        openNIDevice.setMirror(true);
        openNIDevice.addUserGenerator();
        openNIDevice.addHandsGenerator();
        openNIDevice.addAllHandFocusGestures();
        openNIDevice.setMaxNumUsers(3);
        openNIDevice.setMaxNumHands(2);
        openNIDevice.start();
    }


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
                userId = user->getXnID();
//                user->drawSkeleton();

                // rotation
                float rotHandZ = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().z;
                float rotShoulderZ = user->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;
                if( (rotShoulderZ - rotHandZ) > 250 )
                {
                    // left - right
                    float rotVal1 = user->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().x;
                    float rotVal2 = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().x;
                    kinectMovementX = rotVal1 - rotVal2;

                    // up - down
                    float rotVal3 = user->getJoint(JOINT_RIGHT_ELBOW).getWorldPosition().y;
                    float rotVal4 = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().y;
                    kinectMovementY = rotVal3 - rotVal4;
                }
                else
                {
                    kinectMovementX = 0;
                    kinectMovementY = 0;
                }

                // speed
                float speedVal1 = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().z;
                float speedVal2 = user->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;
                if( (speedVal2 - speedVal1) > 200 || (speedVal2 - speedVal1) < -200  )
                {
                    kinectMovementZ = speedVal2 - speedVal1;
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
