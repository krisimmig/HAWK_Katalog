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
        openNIDevice.setMaxNumUsers(4);
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

        (numUsers > 0) ? cursor.trackingUser = true : cursor.trackingUser = false;
        if(numUsers == 0) cursor.calibratingUser = false;

        for (int i = 0; i < numUsers; i++)
        {
            user = &openNIDevice.getTrackedUser(i);
            (user->isCalibrating()) ?  cursor.calibratingUser = true : cursor.calibratingUser = false;

            float rightHandX = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().x;
            float rightHandY = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().y;
            float rightHandZ = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().z;

            cursorXPos = rightHandX;
            cursorYPos = rightHandY;

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
            if( leftDistance > 450 )
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
                int zoomDistance = 80;
                float distanceDifference = handsDistance - previousHandsDistance;
                if(handsDistance > previousHandsDistance && distanceDifference > zoomDistance)
                {
                    changeZoomLevel(1);
                }
                else if(handsDistance < previousHandsDistance && distanceDifference < -zoomDistance)
                {
                    changeZoomLevel(0);
                }
            }

            // set hand drag
            if( (rightShoulderZ - rightHandZ) > 450 && !twoHands && cursor.coordsReady())
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

}

void testApp::exit()
{
    if(useKinect)
    {
        openNIDevice.stop();
    }
}
