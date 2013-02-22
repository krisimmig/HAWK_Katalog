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
    debugGestures = false;
    debugUsers = false;
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
    cursor.isActiveUser = isActiveUser = false;

    activeUserId = -1;

}

void testApp::update()
{
    if(useKinect)
    {
        openNIDevice.update();
        int numUsers = openNIDevice.getNumTrackedUsers();
        int numHands = openNIDevice.getNumTrackedHands();

        if(numUsers == 0) cursor.isActiveUser = isActiveUser = false;

        // -------------
        // USERS
        // -------------
        for (int i = 0; i < numUsers; i++)
        {
            user = &openNIDevice.getTrackedUser(i);
            float userPos = user->getCenter().x;

            if(userPos > -150 && userPos < 150 && user->isSkeleton())
            {
                if(!isActiveUser)
                {
                    activeUserId = i;
                    activeUser = user;
                    activeUserId = cursor.activeUserId = i;
                    isActiveUser = cursor.isActiveUser = true;
                    cursor.activeUserPos = activeUser->getCenter().x;
                    if(debugUsers) cout <<  "switch activeUser. New User: " << ofToString(numUsers) <<  " new activeUser: " << ofToString(activeUserId) << endl;

                }
                kinectGestures();
            }
        }

        if(isActiveUser)
        {
            float activeUserPos = activeUser->getCenter().x;
            if(activeUserPos < -150 || activeUserPos > 150 || activeUserPos == 0.0f)
            {
                if(debugUsers) cout << "deactivate activeUser. activeUser pos outside range." << endl;
                activeUser = NULL;
                isActiveUser = cursor.isActiveUser = false;
                cursor.activeUserId = activeUserId = -1;
                cursor.activeUserPos = -5000;
            }
            else if(numUsers == 0)
            {
                if(debugUsers) cout << "deactivate activeUser. 0 user found." << endl;
                activeUser = NULL;
                isActiveUser = cursor.isActiveUser = false;
                cursor.activeUserId = activeUserId = -1;
                cursor.activeUserPos = -5000;
            }
        }
//  cout << "nU: " << ofToString(numUsers) <<  " nAU: " << ofToString(activeUserId) << " isActiveUser: " << ofToString(isActiveUser) << endl;
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
    float rightShoulderZ = activeUser->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;

    // -------------
    // LEFTHAND MOVEMENT / CURSOR
    // -------------

    ofVec3f rightShoulder = activeUser->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition();
    ofVec3f rightElbow = activeUser->getJoint(JOINT_RIGHT_ELBOW).getWorldPosition();
    ofVec3f rightHand = activeUser->getJoint(JOINT_RIGHT_HAND).getWorldPosition();

    ofVec3f leftHand = activeUser->getJoint(JOINT_LEFT_HAND).getWorldPosition();
    ofVec3f leftShoulder = activeUser->getJoint(JOINT_LEFT_SHOULDER).getWorldPosition();

    float armLength = rightShoulder.distance(rightElbow);
    armLength += rightElbow.distance(rightHand);

    // upadte cursor position
    cursor.update(rightHand.x, rightHand.y, leftHand.x, leftHand.y);
    cursor.activeUserXpos = activeUser->getJoint(JOINT_TORSO).getWorldPosition().x;

    // set right hand drag
    if( rightShoulder.z - rightHand.z > 0.7*armLength)
    {
        cursor.rightHand = true;
    }
    else
    {
        cursor.rightHand = false;
    }

    // set right hand raised
    if(  rightShoulder.z - rightHand.z > 0.3*armLength)
    {
        cursor.rightHandRaised = true;
    }
    else
    {
        cursor.rightHandRaised = false;
    }

    ofVec3f leftElbow = activeUser->getJoint(JOINT_LEFT_ELBOW).getWorldPosition();
    float heightDifference = leftHand.y - leftElbow.y;

    // set left hand
    if( heightDifference > 200 && !cursor.leftHandRaised)
    {
        (cursor.leftHand) ? cursor.leftHand = false : cursor.leftHand = true;
        cursor.leftHandRaised = true;
    }

    // set left hand raised
    if( heightDifference < -200)
    {
        cursor.leftHandRaised = false;
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
