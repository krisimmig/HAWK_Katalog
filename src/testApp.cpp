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
        numberOfMaxUsers = 6;
        openNIDevice.setMaxNumUsers(numberOfMaxUsers);
        openNIDevice.start();
    }

    zoomInGestureTimer = cursor.zoomInGestureTimer = 0;
    zoomOutGestureTimer = cursor.zoomOutGestureTimer = 0;
    zoomGestureDuration = cursor.zoomGestureDuration = 20;
    minZoomGestureDistance = 10;
    swipeTimer = 3;
    swipeLeftTimer, swipeRightTimer, swipeUpTimer, swipeDownTimer = 0;
}

void testApp::update()
{
    if(useKinect)
    {
        openNIDevice.update();
        int numUsers = openNIDevice.getNumTrackedUsers();
        int numHands = openNIDevice.getNumTrackedHands();

        if(numUsers == 0) cursor.isActiveUser = false;

        // -------------
        // USERS
        // -------------
        for (int i = 0; i < numUsers; i++)
        {
            user = &openNIDevice.getTrackedUser(i);

            float userPos = user->getCenter().x;
            cursor.usersPos[i] = userPos;

            if(userPos > -130 && userPos < 130 && user->isSkeleton())
            {
                activeUserId = i;
                activeUser = user;
                activeUserId = cursor.activeUserId = i;

                isActiveUser = cursor.isActiveUser = true;
                cursor.activeUserPos = activeUser->getCenter().x;

                kinectGestures();
            }
            else
            {
                if(i <= activeUserId && activeUserId != -1)
                {
                    activeUser = NULL;
                    isActiveUser = cursor.isActiveUser = false;
                    cursor.activeUserId = -1;
                    cursor.activeUserPos = -5000;
                }
            }
        }
    }


}

void testApp::kinectGestures()
{
    // -------------
    // RIGHTHAND MOVEMENT / CURSOR
    // -------------

    float rightHandX = activeUser->getJoint(JOINT_RIGHT_HAND).getWorldPosition().x;
    float rightHandY = activeUser->getJoint(JOINT_RIGHT_HAND).getWorldPosition().y;
    float rightHandZ = activeUser->getJoint(JOINT_RIGHT_HAND).getWorldPosition().z;
    cursorXPos = rightHandX;
    cursorYPos = rightHandY;
    // upadte cursor position
    cursor.update(cursorXPos, cursorYPos);

    // -------------
    // MOVESCREEN
    // -------------
    // depth check right for handdrag -> movescreen
    float rightShoulderZ = activeUser->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;

    // set hand drag
    if( (rightShoulderZ - rightHandZ) > 450 && !twoHands)
    {
        cursor.cursorDrag = true;
    }
    else
    {
        cursor.cursorDrag = false;
    }

    // -------------
    // SWIPE
    // -------------
    if(cursor.cursorDrag)
    {

        float swipeDistanceX = cursor.smoothRightXPos - previousSmoothRightX;
        // left
        if(cursor.smoothRightXPos < previousSmoothRightX && swipeDistanceX < -20 )
        {
            swipeLeftTimer++;
            if(swipeLeftTimer > swipeTimer)
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_LEFT;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipeLeftTimer = 0;
            }
        } else swipeLeftTimer = 0;

        // right
        if(cursor.smoothRightXPos > previousSmoothRightX  && swipeDistanceX > 20)
        {
            swipeRightTimer++;
            if(swipeRightTimer > swipeTimer)
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_RIGHT;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipeRightTimer = 0;
            }
        } else swipeRightTimer = 0;

        previousSmoothRightX = cursor.smoothRightXPos;

        // up
        float swipeDistanceY = cursor.smoothRightYPos - previousSmoothRightY;
        if(cursor.smoothRightYPos > previousSmoothRightY && swipeDistanceY > 20 )
        {
            swipeUpTimer++;
            if(swipeUpTimer > swipeTimer)
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_UP;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipeUpTimer = 0;
            }
        } else swipeUpTimer = 0;

        // down
        if(cursor.smoothRightYPos < previousSmoothRightY && swipeDistanceY < -20 )
        {
            swipeDownTimer++;
            if(swipeDownTimer > swipeTimer)
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_DOWN;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipeDownTimer = 0;
            }
        } else swipeDownTimer = 0;
        previousSmoothRightY = cursor.smoothRightYPos;
    }

    // -------------
    // ZOOM
    // -------------
    // depth check left for twohanded -> zoomgestures
    float leftHandZ = activeUser->getJoint(JOINT_LEFT_HAND).getWorldPosition().z;
    float leftShoulderZ = activeUser->getJoint(JOINT_LEFT_SHOULDER).getWorldPosition().z;
    float leftZDistance = leftShoulderZ - leftHandZ;

    // smooth coords from cursor
    leftHandX = activeUser->getJoint(JOINT_LEFT_HAND).getWorldPosition().x;
    leftHandY = activeUser->getJoint(JOINT_LEFT_HAND).getWorldPosition().y;

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
        // get smooth coords for hands position
        ofVec2f leftHand = cursor.leftVector;
        ofVec2f rightHand = cursor.moveVector;

        float handsDistance = leftHand.distance(rightHand);

        int distanceDifference = handsDistance - previousHandsDistance;

        if(handsDistance < previousHandsDistance) zoomInGestureTimer = cursor.zoomInGestureTimer = 0;
        if(handsDistance > previousHandsDistance) zoomOutGestureTimer = cursor.zoomOutGestureTimer = 0;

        // zoomin
        if(handsDistance > previousHandsDistance && distanceDifference > minZoomGestureDistance)
        {
            if (zoomInGestureTimer > zoomGestureDuration) changeZoomLevel(ZOOM_IN);
            else
            {
                zoomInGestureTimer++;
                cursor.zoomInGestureTimer = zoomInGestureTimer;
            }
        }

        // zoomout
        else if(handsDistance < previousHandsDistance && distanceDifference < -minZoomGestureDistance)
        {
            if (zoomOutGestureTimer > zoomGestureDuration) changeZoomLevel(ZOOM_OUT);
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
}

void testApp::changeZoomLevel(zoomLevelEnum _zoomLevel)
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
