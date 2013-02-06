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
    swipeTimerDefaultDOWN = swipeDownTimer = 15;
    swipeUpTimer = swipeTimerDefaultY = 8;
    swipingLeft = swipingRight = swipingUp = swipingDown = false;
    zoomOut = zoomIn = false;
    zoomInTimer = zoomOutTimer = zoomTimerDefault = 15;

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
//            cursor.usersPos[i] = userPos;

            if(userPos > -150 && userPos < 150 && user->isSkeleton())
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
//                if(i <= activeUserId && activeUserId != -1)
//                {
                activeUser = NULL;
                isActiveUser = cursor.isActiveUser = false;
                cursor.activeUserId = -1;
                cursor.activeUserPos = -5000;
//                }
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
//    dist1 = smoothActiveUserXPos - cursor.smoothRightXPos;
    dist1 = cursor.activeUserXpos - cursorXPos;
//    cout << "x: " << ofToString(smoothActiveUserXPos) << " hX: " << ofToString(cursor.smoothRightXPos) <<  " - d: " << ofToString(dist1) <<  " || l: " << ofToString(swipingLeft) <<  " t: " << ofToString(swipeLeftTimer) << endl;

    // only if depth reached
    if(cursor.cursorDrag)
    {

        // SWIPE LEFT
        if(dist1 < -320 && !swipingRight && previousRightX > rightHandX)
        {
            swipingLeft = true;
        }
        if(swipingLeft)
        {
            swipeLeftTimer--;
            if(swipeLeftTimer >= 0 && dist1 > 100  && previousRightX > rightHandX)
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
        if(dist1 > 200 && !swipingLeft  && previousRightX < rightHandX)
        {
            swipingRight = true;
        }

        if(swipingRight)
        {
            swipeRightTimer--;
            if(swipeRightTimer >= 0 && dist1 < -270  && previousRightX < rightHandX)
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

        previousRightX = rightHandX;

        // SWIPE UP
//        cout     << "x: " << ofToString(rightHandX) << " tl: " << ofToString(swipeLeftTimer)  << " tr: " << ofToString(swipeRightTimer)
//                 << " || y: " << ofToString(rightHandY) << " tu: " << ofToString(swipeUpTimer)  << " td: " << ofToString(swipeDownTimer)
//                 << endl;

        if(rightHandY > 100 && rightHandY < 550 && !swipingDown && previousRightY < rightHandY && swipeUpTimer == swipeTimerDefaultY)
        {
            swipingUp = true;
            cout << "Swipe UP Start" << endl;
        }
        if(swipingUp)
        {
            swipeUpTimer--;
            if(swipeUpTimer >= 0 && rightHandY > 550 && previousRightY < rightHandY )
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_UP;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipingUp = false;
                swipeUpTimer = swipeTimerDefaultY;
                cout << "Swipe UP Event --" << endl;
            }
            else if(swipeUpTimer <= 0)
            {
                swipingUp = false;
                swipeUpTimer = swipeTimerDefaultY;
            }
        }

        // SWIPE DOWN
        float rightHandHeight = cursor.smoothRightYPos;

        if(rightHandY > 600 && swipeDownTimer == swipeTimerDefaultDOWN && !swipingUp && previousRightY > rightHandY)
        {
            swipingDown = true;
            cout << "Swipe DOWN Start" << endl;
        }
        if(swipingDown)
        {
            swipeDownTimer--;
            if(swipeDownTimer >= 0 && rightHandY < 400  && previousRightY > rightHandY)
            {
                static CustomEvent swipeGestureEvent;
                swipeGestureEvent.swipeDirection = SWIPE_DOWN;
                ofNotifyEvent(CustomEvent::swipeGesture, swipeGestureEvent);
                swipingDown = false;
                swipeDownTimer = swipeTimerDefaultDOWN;
                cout << "Swipe DOWN Event end" << endl;
            }
            else if(swipeDownTimer <= 0)
            {
                swipingDown = false;
                swipeDownTimer = swipeTimerDefaultDOWN;
            }
        }
        previousRightY = rightHandY;
    }
    // if no crusor drag, reset all timers and gesture bools
    else
    {
        swipingDown = swipingUp = swipingRight = swipingLeft = false;
        swipeDownTimer = swipeTimerDefaultDOWN;
        swipeLeftTimer = swipeRightTimer = swipeTimerDefaultX;
        swipeUpTimer = swipeTimerDefaultY;
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
        twoHands = cursor.twoHands = true;
        cursor.updateLeftHanded(leftHandX, leftHandY);
    }
    else
    {
        twoHands = cursor.twoHands = false;
        cursor.emptyLists();
    }

    // check for zoom gesture
//    leftHandX, rightHandX, cursor.activeUserXpos
    float dist2 = cursor.activeUserXpos - leftHandX;
    cout << "dist2: " << ofToString(dist2) << " dist1: " << ofToString(dist1)  << " ot: " << ofToString(zoomOutTimer)   << " it: " << ofToString(zoomInTimer) << endl;
    if(twoHands)
    {
        // ZOOM OUT
        if(dist2 > 600 && dist1 < -350 && !zoomIn && !zoomOut && zoomOutTimer == zoomTimerDefault)
        {
            zoomOut = true;
        }

        if(zoomOut)
        {
            zoomOutTimer--;
            if(zoomOutTimer <= 0 && dist2 < 200 && dist1 > -100)
            {
                static CustomEvent zoomGestureEvent;
                zoomGestureEvent.zoomLevel = ZOOM_OUT;
                ofNotifyEvent(CustomEvent::zoomChange, zoomGestureEvent);
                cout << "zoom out" << endl;
                zoomOut = false;
                zoomOutTimer = zoomTimerDefault;
            }
            else if(zoomOutTimer <= 0)
            {
                zoomOut = false;
                zoomOutTimer = zoomTimerDefault;
            }
        }

        // ZOOM IN
        if(dist2 < 200 && dist1 > -50 && !zoomIn && !zoomOut && zoomInTimer == zoomTimerDefault)
        {
            zoomIn = true;
        }

        if(zoomIn)
        {
            zoomInTimer--;
            if(zoomInTimer <= 0 && dist2 > 350 && dist1 < -150)
            {
                static CustomEvent zoomGestureEvent;
                zoomGestureEvent.zoomLevel = ZOOM_IN;
                ofNotifyEvent(CustomEvent::zoomChange, zoomGestureEvent);
                cout << "zoom in" << endl;
                zoomIn = false;
                zoomOutTimer = zoomTimerDefault;
            }
            else if(zoomInTimer <= 0)
            {
                zoomIn = false;
                zoomInTimer = zoomTimerDefault;
            }
        }
    }
    else
    {
        zoomIn = zoomOut = false;
        zoomInTimer = zoomOutTimer = zoomTimerDefault;
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
