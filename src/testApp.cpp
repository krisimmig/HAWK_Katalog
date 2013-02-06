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
    swipeLeftTimer = swipeRightTimer = swipeTimerDefaultX = 30;
    swipeUpTimer = swipeDownTimer = swipeTimerDefaultY = 8;
    swipingLeft = swipingRight = swipingUp = swipingDown = false;

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
    cursor.activeUserXpos = activeUser->getJoint(JOINT_RIGHT_HIP).getWorldPosition().x;

    // -------------
    // MOVESCREEN
    // -------------
    // depth check right for handdrag -> movescreen
    float rightShoulderZ = activeUser->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;

    // set hand drag
    if( (rightShoulderZ - rightHandZ) > 350 && !twoHands)
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

    // SWIPE DISTANCE
    float smoothActiveUserXPos = cursor.smoothActiveUserXPos;
    float dist1;
    dist1 = smoothActiveUserXPos - cursor.smoothRightXPos;
//    cout << "x: " << ofToString(smoothActiveUserXPos) << " hX: " << ofToString(cursor.smoothRightXPos) <<  " - d: " << ofToString(dist1) <<  " || l: " << ofToString(swipingLeft) <<  " t: " << ofToString(swipeLeftTimer) << endl;



    // SWIPE LEFT
    if(dist1 < -320 && !swipingRight)
    {
        swipingLeft = true;
    }
    if(swipingLeft)
    {
        swipeLeftTimer--;
        if(swipeLeftTimer >= 0 && dist1 > 180)
        {
            static CustomEvent swipeGestureEvent;
            swipeGestureEvent.swipeDirection = SWIPE_LEFT;
            ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
            swipingLeft = false;
            swipeLeftTimer = swipeTimerDefaultX;
            cout << "SWIPE LEFT!" << endl;
        }
        else if(swipeLeftTimer <= 0)
        {
            swipingLeft = false;
            swipeLeftTimer = swipeTimerDefaultX;
        }
    }

    // SWIPE RIGHT
    if(dist1 > 230 && !swipingLeft)
    {
        swipingRight = true;
    }

    if(swipingRight)
    {
        swipeRightTimer--;
        if(swipeRightTimer >= 0 && dist1 < -270)
        {
            static CustomEvent swipeGestureEvent;
            swipeGestureEvent.swipeDirection = SWIPE_RIGHT;
            ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
            swipingRight = false;
            swipeRightTimer = swipeTimerDefaultX;
            cout << "SWIPE RIGHT!" << endl;
        }
        else if(swipeRightTimer <= 0)
        {
            swipingRight = false;
            swipeRightTimer = swipeTimerDefaultX;
        }
    }

    // SWIPE UP
    if((rightShoulderZ - rightHandZ) > 350 )
    {
        cout << "y: " << ofToString(rightHandY) << " td: " << ofToString(swipeUpTimer)  << " tu: " << ofToString(swipeDownTimer) << endl;
        if(rightHandY > 100 && rightHandY < 550 && !swipingDown && previousSmoothRightY < rightHandY && swipeUpTimer == swipeTimerDefaultY)
        {
            swipingUp = true;
//            cout << "Swipe UP Start" << endl;
        }
        if(swipingUp)
        {
            swipeUpTimer--;
            if(swipeUpTimer >= 0 && rightHandY > 550)
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_UP;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipingUp = false;
                swipeUpTimer = swipeTimerDefaultY;
//                cout << "Swipe UP Event --" << endl;
            }
            else if(swipeUpTimer <= 0)
            {
                swipingUp = false;
                swipeUpTimer = swipeTimerDefaultY;
            }
        }

        // SWIPE DOWN
        float rightHandHeight = cursor.smoothRightYPos;

        if(rightHandY > 450 && swipeDownTimer == swipeTimerDefaultY && !swipingUp && previousSmoothRightY > rightHandY)
        {
            swipingDown = true;
//            cout << "Swipe DOWN Start" << endl;
        }
        if(swipingDown)
        {
            swipeDownTimer--;
            if(swipeDownTimer >= 0 && rightHandY < 250)
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_DOWN;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipingDown = false;
                swipeDownTimer = swipeTimerDefaultY;
//                cout << "Swipe DOWN Event --" << endl;
            }
            else if(swipeDownTimer <= 0)
            {
                swipingDown = false;
                swipeDownTimer = swipeTimerDefaultY;
            }
        }
        previousSmoothRightY = rightHandY;
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

    if( leftZDistance > 250 )
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
