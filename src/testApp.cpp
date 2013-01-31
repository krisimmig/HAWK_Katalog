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

    zoomInGestureTimer = cursor.zoomInGestureTimer = 0;
    zoomOutGestureTimer = cursor.zoomOutGestureTimer = 0;
    gestureDuration = cursor.gestureDuration = 28;
    minZoomGestureDistance = 12;
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

            leftHandX = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().x;
            leftHandY = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().y;

            // depth check right
            float rightShoulderZ = user->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;

            // depth check left
            float leftHandZ = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().z;
            float leftShoulderZ = user->getJoint(JOINT_LEFT_SHOULDER).getWorldPosition().z;
            float leftZDistance = leftShoulderZ - leftHandZ;
            if( leftZDistance > 450 )
            {
                twoHands = true;
                cursor.updateLeftHanded(leftHandX, leftHandY);
            }
            else
            {
                twoHands = false;
                cursor.emptyLists();
            }

            // check for zoom gesture
            if(twoHands)
            {

                ofVec2f leftHand = cursor.leftVector;
                ofVec2f rightHand = cursor.moveVector;

                float handsDistance = leftHand.distance(rightHand);

                int distanceDifference = handsDistance - previousHandsDistance;
//                cout    << "l: " << ofToString(leftHand.x)  << "/" << ofToString(leftHand.y)  << " r: " << ofToString(rightHand.x)  << "/" << ofToString(rightHand.y)
//                        << "  || Dis: " << ofToString(handsDistance)  << " pD: " << ofToString(previousHandsDistance)
//                        << "  || Dif: " << ofToString(distanceDifference)
//                        << "  || Out: " << ofToString(zoomOutGestureTimer) << " In: " << ofToString(zoomInGestureTimer) << endl;

                if(handsDistance < previousHandsDistance) zoomInGestureTimer = cursor.zoomInGestureTimer = 0;
                if(handsDistance > previousHandsDistance) zoomOutGestureTimer = cursor.zoomOutGestureTimer = 0;

                // zoomin
                if(handsDistance > previousHandsDistance && distanceDifference > minZoomGestureDistance)
                {
                    if (zoomInGestureTimer > gestureDuration) changeZoomLevel(1);
                    else
                    {
                        zoomInGestureTimer++;
                        cursor.zoomInGestureTimer = zoomInGestureTimer;
                    }
                }

                // zoomout
                else if(handsDistance < previousHandsDistance && distanceDifference < -minZoomGestureDistance)
                {
                    if (zoomOutGestureTimer > gestureDuration) changeZoomLevel(0);
                   else
                    {
                        zoomOutGestureTimer++;
                        cursor.zoomOutGestureTimer = zoomOutGestureTimer;
                    }
                }
                previousHandsDistance = handsDistance;
            }
            else
            {
                zoomInGestureTimer = cursor.zoomInGestureTimer = 0;
                zoomOutGestureTimer = cursor.zoomOutGestureTimer = 0;
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
