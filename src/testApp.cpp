#include "testApp.h"
#include "stdio.h"

testApp::testApp()
{
    cursor.setup(0,0);
    viewmanager.setCursor(cursor);
}

void testApp::setup()
{
    ofEnableSmoothing();
    ofSetFrameRate(30);
    float filterFactor = 0.1f;

    garamondRegularH1.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 25);

    // use app with or without kinect
    useKinect = true;
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
        openNIDevice.setUserSmoothing(6);
        openNIDevice.setMaxNumHands(2);
        openNIDevice.start();
    }

}

void testApp::update()
{
    if(useKinect)
    {
        openNIDevice.update();
        int numUsers = openNIDevice.getNumTrackedUsers();
        int numHands = openNIDevice.getNumTrackedHands();

        // iterate through users
        for (int i = 0; i < numHands; i++)
        {

            // get a reference to this user
            ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);

            // get hand position
            ofPoint & handPosition = hand.getPosition();
            // set cursor Position & adjust to screensize
            cursorXPos = ( handPosition.x / (600 / 100) ) * (ofGetWidth() / 100);
            cursorYPos = ( handPosition.y / (440 / 100) ) * (ofGetHeight() / 100);
        }

        for (int i = 0; i < numUsers; i++)
        {
            user = &openNIDevice.getTrackedUser(i);
            float rightHandX = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().x;
            float rightHandY = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().y;
            float rightHandZ = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().z;

            // depth check right
            float rightShoulderZ = user->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;

            // depth check left
            float leftHandX = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().x;
            float leftHandY = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().z;
            float leftHandZ = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().z;

            ofVec3f leftHand;
            ofVec3f rightHand;

            leftHand.set(leftHandX,leftHandY,leftHandZ);
            rightHand.set(rightHandX,rightHandY,rightHandZ);

            handsDistance = leftHand.distance(rightHand);

            float leftShoulderZ = user->getJoint(JOINT_LEFT_SHOULDER).getWorldPosition().z;
            float leftDistance = leftShoulderZ - leftHandZ;
            if( leftDistance > 380 )
            {
                twoHands = true;
            }
            else
            {
                twoHands = false;
            }

            // check for zoom gesture
            if(twoHands)
            {
                float distanceDifference = handsDistance - previousHandsDistance;
                if(handsDistance > previousHandsDistance && distanceDifference > 100)
                {
                    changeZoomLevel(1);
                    cout << "zoom in from testApp ******** " << endl;
                }
                else if(handsDistance < previousHandsDistance && distanceDifference < -100)
                {
                    changeZoomLevel(0);
                    cout << "******** zoom out from testApp" << endl;
                }
            }

            // set hand drag
            if( (rightShoulderZ - rightHandZ) > 450 && !twoHands)
            {
                cursor.cursorDrag = true;
            }
            else
            {
                cursor.cursorDrag = false;
            }

            previousHandsDistance = handsDistance;
        }
        // upadte cursor position
        cursor.update(cursorXPos, cursorYPos);

    }
}

void testApp::changeZoomLevel(int _zoomLevel)
{
    static CustomEvent changeZoomLevel;
    changeZoomLevel.zoomLevel = _zoomLevel;
    ofNotifyEvent(CustomEvent::zoomChange, changeZoomLevel);
}

void testApp::draw()
{
//    if(trackingHand)
//    {
//        SetCursorPos(cursorXPos + cursorRadius/2, cursorYPos + cursorRadius/2);
//    }
//    if (cursor.visible)
//    {
//        ofShowCursor();
//    }
//    else
//    {
//        ofHideCursor();
//    }
//    cout << "testApp cursor: " << ofToString(cursor.cursorDrag) << endl;
}

void testApp::exit()
{
    if(useKinect)
    {
        openNIDevice.stop();
    }
}
