#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyReleased, this, &WorldView::keyReleased);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofAddListener(CustomEvent().zoomChange, this, &WorldView::listenerZoomChange);
    ofAddListener(CustomEvent().swipeGesture, this, &WorldView::listenerSwipeGesture);

    fontXL.loadFont("fonts/MyriadPro-Regular_0.otf", 50, true, true);
    fontLBold.loadFont("fonts/MyriadPro-Bold_0.otf", 26, true, true);
    fontLLight.loadFont("fonts/MyriadPro-Light.otf", 26, true, true);
    fontL.loadFont("fonts/MyriadPro-Regular_0.otf", 26, true, true);
    fontS.loadFont("fonts/MyriadPro-Regular_0.otf", 18, true, true);
    fontSLight.loadFont("fonts/MyriadPro-Light.otf", 18, true, true);
    fontSLLight.loadFont("fonts/MyriadPro-Light.otf", 21, true, true);
    ofEnableSmoothing();

    // gestures
    gestureTimerZoom = 0;
    gestureTimerSwipe = 0;
    gestureTimeout = 30;

    // camera
    cameraHeight = -1500;
    ofVec3f v1;
    ofVec3f v2;
    v1.set(0,1,0);
    v2.set(0,0,1);
    camera.rotate(180, v1);
    camera.rotate(180, v2);
    currentCameraHeight = cameraHeight;
    camera.setPosition(0,0,currentCameraHeight);
    zoomLevel = 3;
    dragSpeed = 30.0f;
    currentYDragSpeed = dragSpeed;
    speedXCounter = 0;
    speedYCounter = 0;
    speedFactor = 1.5f;

    // currentDepartmentString setup
    fachbereichArray = {FACHBEREICH_AD,FACHBEREICH_CICD,FACHBEREICH_DM,FACHBEREICH_FG,FACHBEREICH_GD,FACHBEREICH_IAID,FACHBEREICH_LD,FACHBEREICH_MG,FACHBEREICH_PD};
    departmentChangedToLeft = departmentChangedToRight = false;
    totalDeptNumber = 8; // 10 ?!?
    currentDeptNumber = 0; // start at AD
    currentDepartmentString = Students::convertFBEnumToString(fachbereichArray[currentDeptNumber]);
    numberOfStudents = Students::countAll(fachbereichArray[currentDeptNumber]);
    currentStudent = -1;
    random10 = ofRandom(10);
    firstStart = true;

    // studentObjects
    studentObjectAbstandX = 146;
    studentObjectAbstandY = 174;
    studentObjectsXPos = studentObjectsYPos = studentObjectsXPosDefault = studentObjectsYPosDefault = 0;

    // info panel
    shakingTimer = 0;
    justArrived = infoPanelToLeft = infoPanelToRight = leavingInfoPanelView = studentSwitched = false;
    projectImagesYPosition = 0;
    currentProjectImagesYPosition = 0;
    currentImageHeight = 0;
    currentImageNumber = 1;
    infoPanelWidth = 1100;
    infoPanelXPosition = infoPanelFinalXPosition = ofGetWidth()/2 - infoPanelWidth * 0.25;
    infoPanelResetPosition = infoPanelXPosition;
    infoPanelFinalYPosition = 0;
    infoPanelYPosition = 0;
    totalImageColumnHeight = 0;

    // menu
    menuActivated = false;
    menuActive = false;
    justZoomed = false;
    cursorPointer = cursorPointerTop = false;

    // load audio
    menuOpen.loadSound(AUDIO_DIR "/menuOpen.wav");
    menuClick.loadSound(AUDIO_DIR "/menuClick.wav");
    error.loadSound(AUDIO_DIR "/error.wav");

    // load grafix
    bottomInterface.loadImage(GRAFIX_DIR "/grafix_bottomInterface.png");
    cursor_handClosed.loadImage(GRAFIX_DIR "/grafix_handClosed.png");
    cursor_handOpened.loadImage(GRAFIX_DIR "/grafix_handOpened.png");
    cursor_handPointer.loadImage(GRAFIX_DIR "/grafix_handPointer.png");
    auswahlMitte.loadImage(GRAFIX_DIR "/grafix_auswahlMitte.png");
    auswahlMitte_hover.loadImage(GRAFIX_DIR "/grafix_auswahlMitte_hover.png");
    fachbereichTitel.loadImage(GRAFIX_DIR "/grafix_fachbereichTitel.png");
    gridBackground.loadImage(GRAFIX_DIR "/grafix_gridBackground.png");
    auswahlOben.loadImage(GRAFIX_DIR "/grafix_auswahlOben.png");
    auswahlOben_hover.loadImage(GRAFIX_DIR "/grafix_auswahlOben_hover.png");
    ubersichtOben.loadImage(GRAFIX_DIR "/grafix_ubersichtOben.png");
    ubersichtOben_hover.loadImage(GRAFIX_DIR "/grafix_ubersichtOben_hover.png");
    fachbereicheUnten.loadImage(GRAFIX_DIR "/grafix_fachbereicheUnten.png");
    fachbereicheUnten_hover.loadImage(GRAFIX_DIR "/grafix_fachbereicheUnten_hover.png");
}

WorldView::~WorldView()
{
    //dtor
    ofRemoveListener(ofEvents().draw, this, &WorldView::draw);
    ofRemoveListener(ofEvents().update, this, &WorldView::update);
    ofRemoveListener(ofEvents().keyReleased, this, &WorldView::keyReleased);
    delete studentObjects;
}

void WorldView::update(ofEventArgs &e)
{
    // initialize
    if(firstStart)
    {
        updateDepartment();
        firstStart = false;
    }

    // screen position
    updateScreenPosition();

    // detail info panel view
    if(zoomLevel == 1)
    {
        shakeInfoPanel();
        updateInfoPanelPosition();
    }

    if(zoomLevel == 3) updateStudenObjectsPosition();

    updateZoomLevel();

    // menu activation
    if(cursor->leftHand && !menuActive && cursor->rightHandRaised)
    {
        menuActivated = true;
        cursor->leftHand = false;
    }
    else if(cursor->leftHand) cursor->leftHand = false;

    // zoom gesture timer
    if(gestureTimerZoom > 0)
    {
        gestureTimerZoom--;
    }
    // swipe gesture timer
    if(gestureTimerSwipe > 0)
    {
        gestureTimerSwipe--;
    }
}

void WorldView::updateStudenObjectsPosition()
{

    // animate to left
    if(departmentChangedToLeft)
    {
        if(studentObjectsXPosFuture <= studentObjectsXPos && studentObjectsXPosFuture != studentObjectsXPosDefault)
        {
            studentObjectsXPos -= 240;
        }
        else if(studentObjectsXPos < studentObjectsXPosFuture && studentObjectsXPosFuture != studentObjectsXPosDefault)
        {
            studentObjectsXPos = 2000.0f;
            studentObjectsXPosFuture = studentObjectsXPosDefault;
            currentDeptNumber++;
            updateDepartment();
        }
        else if(studentObjectsXPos > 3.0f)
        {
            studentObjectsXPos = studentObjectsXPos*0.75f;
        }
        else
        {
            departmentChangedToLeft = false;
        }
    }

    // animate to right
    if(departmentChangedToRight)
    {
        if(studentObjectsXPosFuture >= studentObjectsXPos && studentObjectsXPosFuture != studentObjectsXPosDefault)
        {
            studentObjectsXPos += 240;
        }
        else if(studentObjectsXPos > studentObjectsXPosFuture && studentObjectsXPosFuture != studentObjectsXPosDefault)
        {
            studentObjectsXPos = -2000.0f;
            studentObjectsXPosFuture = studentObjectsXPosDefault;
            currentDeptNumber--;
            updateDepartment();
        }
        else if(studentObjectsXPos < -3.0f)
        {
            studentObjectsXPos = studentObjectsXPos*0.75f;
        }
        else
        {
            departmentChangedToRight = false;
        }
    }
}

void WorldView::updateDepartment()
{
    // delete old objects if not first time here
    if(!firstStart)
    {
        delete[] studentObjects;
        delete studentIdArray;
    }

    // make new objects arrays
    numberOfStudents = Students::countAll(fachbereichArray[currentDeptNumber]);
    studentObjects = new Object3D*[numberOfStudents];
    studentIdArray = Students::getStudentIds(fachbereichArray[currentDeptNumber]);

    // setup objects
    int counter = 0;
    int width = sqrt(numberOfStudents) * studentObjectAbstandX;
    int height = sqrt(numberOfStudents) * studentObjectAbstandY;
    for(int i = 0; i < numberOfStudents; i++)
    {
        for(int j = 0; j < sqrt(numberOfStudents); j++)
        {
            if(counter < numberOfStudents)
            {
                int x = studentObjectAbstandX * j - width/2;
                int y = studentObjectAbstandY * i - height/2;
                int z = 0;
                studentObjects[counter] = new Object3D();
                studentObjects[counter]->setup(x,y,z, counter, studentIdArray[counter]);
                studentObjects[counter]->setFont(fontXL);
                counter++;
            }
        }
    }
}

void WorldView::updateInfoPanelPosition()
{
    // coming from zoomIN
    if(justArrived)
    {
        infoPanelYPosition = 2000;
        justArrived = false;
    }
    if(infoPanelYPosition > 10 && !leavingInfoPanelView) infoPanelYPosition *= 0.8f;

    // leaving infopanelview
    if(leavingInfoPanelView && infoPanelYPosition > -2000) infoPanelYPosition -= 100;
    if(infoPanelYPosition <= -2000)
    {
        leavingInfoPanelView = false;
        zoomLevel = 3;
        currentCameraHeight = cameraHeight;
    }

    // move project images on Y-axis
    currentProjectImagesYPosition += currentYDragSpeed * 2.3f;

    // reset project images column Y position after scrolling too far UP
    if(!cursor->rightHand && currentProjectImagesYPosition < ofGetHeight()*0.75f-totalImageColumnHeight || currentProjectImagesYPosition > 0) currentProjectImagesYPosition *= 0.9f;

    // move infopanel on X-axis, only if xspeed high enough
    if(cursor->rightHand && fabs(currentXDragSpeed) > 5.0f && !infoPanelToRight && !infoPanelToLeft) infoPanelXPosition -= currentXDragSpeed * 2.3f;

    // infopanel not moving, not switching to new student & not in reset position -> reposition
    float difference = infoPanelXPosition - infoPanelResetPosition;
    if(!cursor->rightHand && !infoPanelToRight && !infoPanelToLeft && fabs(difference) > 1.0f)  infoPanelXPosition -= difference*0.2f;

    // switch student
    if(cursor->rightHand && difference < -200 && !infoPanelToRight && !infoPanelToLeft) swipeGestureEvent(SWIPE_LEFT);
    if(cursor->rightHand && difference > 200 && !infoPanelToRight && !infoPanelToLeft) swipeGestureEvent(SWIPE_RIGHT);

    // swipe to left infopanel position
    if(infoPanelToLeft && !infoPanelToRight)
    {
        float distance = infoPanelXPosition - infoPanelResetPosition;
        if(infoPanelXPosition > - infoPanelWidth && infoPanelXPosition <= infoPanelResetPosition) infoPanelXPosition -= 150;
        else if(infoPanelXPosition <= - infoPanelWidth && !studentSwitched)
        {
            infoPanelXPosition = ofGetWidth() + infoPanelWidth;
            currentProjectImagesYPosition = 0;
            currentImageNumber = 1;
            currentStudent++;
            studentSwitched = true;
        }

        else if(distance > 2.5f) infoPanelXPosition -= distance * 0.2f;
        else
        {
            infoPanelXPosition = infoPanelResetPosition;
            infoPanelToLeft = studentSwitched = false;
        }
    }

    // swipe to right infopanel position
    if(infoPanelToRight && !infoPanelToLeft)
    {
        float distance = infoPanelXPosition - infoPanelResetPosition;
        if(distance < 0) distance *= -1;
        if(infoPanelXPosition < ofGetWidth() && infoPanelXPosition >= infoPanelResetPosition) infoPanelXPosition += 150;
        else if(infoPanelXPosition >= ofGetWidth()  && !studentSwitched)
        {
            infoPanelXPosition = -infoPanelWidth;
            currentProjectImagesYPosition = 0;
            currentImageNumber = 1;
            currentStudent--;
            studentSwitched = true;
        }

        else if(distance > 2.5f) infoPanelXPosition += distance * 0.2f;
        else
        {
            infoPanelXPosition = infoPanelResetPosition;
            infoPanelToRight = studentSwitched = false;
        }
    }
}

void WorldView::updateScreenPosition()
{
    // center in overview
    if(zoomLevel == 3 && !cursor->rightHand)
    {
        if(fabs(camera.getGlobalPosition().x) > 1.0f) camera.truck(-camera.getGlobalPosition().x * 0.1f);
        if(fabs(camera.getGlobalPosition().y) > 1.0f) camera.boom(camera.getGlobalPosition().y * 0.1f);
    }

    // change speed to new zoomlevel and activate justarrived bool
    switch(zoomLevel)
    {
    case 2:
        speedFactor = 0.5f;
        break;
    case 3:
        speedFactor = 1.5f;
        break;
    }

    // calcualte screen move speed
    if(cursor->rightHand && !cursor->leftHand)
    {
        // X
        float difference = cursor->smoothRightXPos - pmouseX;
        if(difference > -30.0f && difference < 30.0f) currentXDragSpeed = -difference * speedFactor;

        // Y
        difference = cursor->smoothRightYPos - pmouseY;
        if(difference > -30.0f && difference < 30.0f) currentYDragSpeed = -difference  * speedFactor;
    }

    pmouseX = cursor->smoothRightXPos;
    pmouseY = cursor->smoothRightYPos;

    // calculate kinetic scrolling (kinetic camera movement only in zoomLevel 2)
    // if dragSpeed slower than +-0.5, set to 0, else mulitply by 0.9
    if(!cursor->rightHand && zoomLevel != 3)
    {
        // X
        if(currentXDragSpeed < -0.5f || currentXDragSpeed > 0.5f)
        {
            currentXDragSpeed *= 0.9f;
            if(zoomLevel == 2) camera.truck(currentXDragSpeed);
        }
        else
        {
            currentXDragSpeed = 0.0f;
        }

        // Y
        if(currentYDragSpeed < -0.5f || currentYDragSpeed > 0.5f)
        {
            currentYDragSpeed *= 0.9f;
            if(zoomLevel == 2) camera.boom(currentYDragSpeed);
        }
        else currentYDragSpeed = 0.0f;
    }

    // center on closest in zoomlevel 2
    if(zoomLevel == 2 && !cursor->rightHand && !zooming)
    {
        if(fabs(currentXDragSpeed) < 1.2f && fabs(currentYDragSpeed) < 1.2f )
        {
            float distanceX = camera.getPosition().x - closestObjectVector.x;
            float distanceY = camera.getPosition().y - closestObjectVector.y;
            if(distanceX < -1.0f || distanceX > 1.0f)
            {
                distanceX *= 0.1f;
                camera.truck(-distanceX);
            }
            if(distanceY < -1.0f || distanceY > 1.0f)
            {
                distanceY *= 0.1f;
                camera.boom(distanceY);
            }
        }
    }

    // center studentObjects
    if(zoomLevel == 3 && !cursor->rightHand)
    {
        if(studentObjectsXPos < -0.5f || studentObjectsXPos > 0.5f)
        {
            studentObjectsXPos *= 0.9f;
        }
    }
}

void WorldView::updateZoomLevel()
{
    // zoomout
    if(camera.getPosition().z > currentCameraHeight)
    {
        zooming = true;
        camera.dolly(50);
    }
    // zoomin
    else if(camera.getPosition().z < currentCameraHeight)
    {
        zooming = true;
        camera.dolly(-50);
    }
    else zooming = false;

    // set zoomlevel for obejcts
    for(int i = 0; i < numberOfStudents; i++)
    {
        studentObjects[i]->setZoomLevel(zoomLevel);
    }
}

void WorldView::draw(ofEventArgs &e)
{
    ofFill();
    ofBackgroundGradient(ofColor(215), ofColor(165));

    camera.begin();

    // draw grid background
    ofPushMatrix();
    ofTranslate(studentObjectsXPos, studentObjectsYPos);

    ofEnableAlphaBlending();
    int counter = 0;
    int zusatz = 2;
    int width = sqrt(numberOfStudents) * studentObjectAbstandX;
    int height = sqrt(numberOfStudents) * studentObjectAbstandY;
    for(int i = -zusatz; i < int(numberOfStudents/sqrt(numberOfStudents)+0.5f) + zusatz; i++)
    {
        for(int j = -zusatz; j < sqrt(numberOfStudents)+zusatz; j++)
        {
            int x = studentObjectAbstandX * j - width/2;
            int y = studentObjectAbstandY * i - height/2;
            int z = 0;
            int rahmen = 6;

            ofSetColor(255,255,255,5);
            ofRect(x-rahmen,y-rahmen,z,114 + rahmen*2, 146 + rahmen*2);
            ofNoFill();
            ofSetColor(100,100,100,15);
            ofRect(x-rahmen,y-rahmen,z,114 + rahmen*2, 146 + rahmen*2);
            ofFill();
        }
    }

    zusatz = 1;
    for(int i = -zusatz; i < int(numberOfStudents/sqrt(numberOfStudents)+0.5f) + zusatz; i++)
    {
        for(int j = -zusatz; j < sqrt(numberOfStudents)+zusatz; j++)
        {
            int x = studentObjectAbstandX * j - width/2;
            int y = studentObjectAbstandY * i - height/2;
            int z = 0;
            int rahmen = 6;

            ofSetColor(255,255,255,5);
            ofRect(x-rahmen,y-rahmen,z,114 + rahmen*2, 146 + rahmen*2);
            ofNoFill();
            ofSetColor(100,100,100,15);
            ofRect(x-rahmen,y-rahmen,z,114 + rahmen*2, 146 + rahmen*2);
            ofFill();
        }
    }

    // draw all student portraits

    for(int i = 0; i < numberOfStudents; i++)
    {
        studentObjects[i]->draw();
    }


    ofPopMatrix();
    kinectMove();
    ofDisableAlphaBlending();
    camera.end();

    drawInfo();
    drawBottomInterface();

    if(!zooming && cursor->isActiveUser) drawHandIndicator();


    ofSetColor(10,10,10);
    std::string umlaute = "1. äöü 2. ÄÖÜ Umlaute";
    std::string umlaut = "ä";
    cout << "ulaute " << ofToChar(umlaut) << endl;
    ofDrawBitmapString(umlaute, 50,50);
    fontL.drawString(umlaute, 50,100);

}

void WorldView::shakeInfoPanel()
{
    if(shakingTimer > 0)
    {
        if(!error.getIsPlaying()) error.play();
        infoPanelXPosition += ofRandom(-15,15);
        shakingTimer--;
    }
    else
    {
        shakingTimer = 0;
    }
}

void WorldView::drawHandIndicator()
{
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.4 , ofGetHeight() * 0.9);
    ofEnableAlphaBlending();
    float x = cursor->smoothRightXPos;
    float y = -cursor->smoothRightYPos;

    // active menu
    if(menuActivated)
    {
        xPosLeftHandMenu = x;
        yPosLeftHandMenu = y;
        menuMiddle.set(xPosLeftHandMenu, -yPosLeftHandMenu);
        menuActivated = false;
        menuActive = true;
        if(!menuOpen.getIsPlaying()) menuOpen.play();
    }

    if(menuActive && menuMiddle.distance(cursor->moveVector) > 280)
    {
        menuActive = false;
        cursor->rightHand = false;
    }

    if(menuActive)
    {
        ofPushMatrix();
        ofTranslate(xPosLeftHandMenu, yPosLeftHandMenu);
        int innerCircleRadius = 60;
        int xCorrection = 10;
        int yCorrection = 10;

        switch(zoomLevel)
        {
        case 3:
            if(x < xPosLeftHandMenu - 50)
            {
                cursorPointer = true;
                ofSetColor(255,255,255);
                auswahlMitte_hover.draw(-auswahlMitte_hover.getWidth() - innerCircleRadius,-auswahlMitte.getHeight()/2);
                if(cursor->rightHand && !zooming)
                {
                    changeZoomLevel(ZOOM_IN);
                    if(!menuClick.getIsPlaying()) menuClick.play();
                    cursorPointer = false;
                }
            }
            else
            {
                cursorPointer = false;
                ofSetColor(255,255,255);
                auswahlMitte.draw(-auswahlMitte.getWidth() - innerCircleRadius,-auswahlMitte.getHeight()/2);
            }

            break;
        case 2:
            // upper button -> detailansicht
            if(x < xPosLeftHandMenu - 50 && y < yPosLeftHandMenu - 20 && y > yPosLeftHandMenu - auswahlOben_hover.getHeight() - 40  && currentStudent != -1)
            {
                cursorPointerTop = true;
                ofSetColor(255,255,255);
                auswahlOben_hover.draw(-auswahlOben_hover.getWidth() - innerCircleRadius + xCorrection,-auswahlOben_hover.getHeight() - yCorrection);
                if(cursor->rightHand && !zooming)
                {
                    changeZoomLevel(ZOOM_IN);
                    if(!menuClick.getIsPlaying()) menuClick.play();
                    cursorPointerTop = false;
                }
            }
            else
            {
                cursorPointerTop = false;
                ofSetColor(255,255,255);
                auswahlOben.draw(-auswahlOben.getWidth() - innerCircleRadius + xCorrection,-auswahlOben_hover.getHeight() - yCorrection);
            }

            // lower button -> fachbereiche
            if(x < xPosLeftHandMenu - 50 && y > yPosLeftHandMenu + 20 && y < yPosLeftHandMenu + fachbereicheUnten.getHeight() + 40 )
            {
                cursorPointer = true;
                ofSetColor(255,255,255);
                fachbereicheUnten_hover.draw(-fachbereicheUnten_hover.getWidth() - innerCircleRadius + xCorrection,yCorrection);
                if(cursor->rightHand && !zooming)
                {
                    changeZoomLevel(ZOOM_OUT);
                    if(!menuClick.getIsPlaying()) menuClick.play();
                    cursorPointer = false;
                }
            }
            else
            {
                cursorPointer = false;
                ofSetColor(255,255,255);
                fachbereicheUnten.draw(-fachbereicheUnten.getWidth() - innerCircleRadius + xCorrection,yCorrection);
            }
            break;
        case 1:
            // upper button -> detailansicht
            if(x < xPosLeftHandMenu - 50 && y < yPosLeftHandMenu - 20 && y > yPosLeftHandMenu - ubersichtOben_hover.getHeight() - 40  && currentStudent != -1)
            {
                cursorPointerTop = true;
                ofSetColor(255,255,255);
                ubersichtOben_hover.draw(-ubersichtOben_hover.getWidth() - innerCircleRadius + xCorrection,-ubersichtOben_hover.getHeight() - yCorrection);
                if(cursor->rightHand && !zooming)
                {
                    changeZoomLevel(ZOOM_OUT);
                    if(!menuClick.getIsPlaying()) menuClick.play();
                    cursorPointerTop = false;
                }
            }
            else
            {
                cursorPointerTop = false;
                ofSetColor(255,255,255);
                ubersichtOben.draw(-ubersichtOben.getWidth() - innerCircleRadius + xCorrection,-ubersichtOben.getHeight() - yCorrection);
            }

            // lower button -> fachbereiche
            if(x < xPosLeftHandMenu - 50 && y > yPosLeftHandMenu + 20 && y < yPosLeftHandMenu + fachbereicheUnten.getHeight() + 40 )
            {
                cursorPointer = true;
                ofSetColor(255,255,255);
                fachbereicheUnten_hover.draw(-fachbereicheUnten_hover.getWidth() - innerCircleRadius + xCorrection,yCorrection);
                if(cursor->rightHand && !zooming)
                {
                    changeZoomLevel(ZOOM_RESET);
                    if(!menuClick.getIsPlaying()) menuClick.play();
                    cursorPointer = false;
                }
            }
            else
            {
                cursorPointer = false;
                ofSetColor(255,255,255);
                fachbereicheUnten.draw(-fachbereicheUnten.getWidth() - innerCircleRadius + xCorrection,yCorrection);
            }
            break;
        }

        // draw center circle
        ofSetColor(255,255,255);
        ofNoFill();
        ofEllipse(0, 0, innerCircleRadius*2,innerCircleRadius*2);
        ofEllipse(0, 0, innerCircleRadius*2+1,innerCircleRadius*2+1);
        ofEllipse(0, 0, innerCircleRadius*2+2,innerCircleRadius*2+2);
        ofEllipse(0, 0, innerCircleRadius*2+3,innerCircleRadius*2+3);
        ofFill();
        ofSetColor(255,255,255,100);
        ofEllipse(0, 0, innerCircleRadius*2,innerCircleRadius*2);

        ofPopMatrix();
    }


    // hand position indicator
    ofSetColor(255,255,255,255);
    if(cursor->rightHandRaised && !cursor->rightHand && cursorPointer || cursorPointerTop) cursor_handPointer.draw(x,y);
    else if (cursor->rightHandRaised && !cursor->rightHand) cursor_handOpened.draw(x,y);

    if(cursor->rightHand)
    {
        if(cursorPointer || cursorPointerTop) cursor_handPointer.draw(x,y);
        else cursor_handClosed.draw(x,y);
    }

    ofDisableAlphaBlending();
    ofPopMatrix();
}

void WorldView::drawInfo()
{
    int previousHeight = 50;
    int imageHeight = 0;
    totalImageColumnHeight = 0;
    int abstandDeptLabels;
    float nameBoxWidth;
    std::string vorName;
    std::string nachName;
    std::string titel;

    switch(zoomLevel)
    {
        // student detail view
    case 1:
        // draw opaque background
        ofEnableAlphaBlending();
        ofSetColor(200,200,200, 230);
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        ofDisableAlphaBlending();

        ofPushMatrix();
        ofTranslate(infoPanelXPosition, infoPanelYPosition);

        ofFill();

        // draw project images
        ofPushMatrix();
        ofTranslate(0, 0 + currentProjectImagesYPosition);

        // draw  right infocards
        for(int i = 0; i <= studentObjects[currentStudent]->totalNumberProjectImages; i++)
        {
            totalImageColumnHeight += studentObjects[currentStudent]->getProjectImageSize(i) + 30;
        }
        ofSetColor(255,255,255);
        ofRect(0,25,850,totalImageColumnHeight);
        // draw right infocard border
        ofSetColor(170,170,170);
        ofNoFill();
        ofRect(0,25,850,totalImageColumnHeight);
        ofFill();
        for(int i = 0; i < studentObjects[currentStudent]->totalNumberProjectImages; i++)
        {
            if(i > 0) imageHeight = studentObjects[currentStudent]->getProjectImageSize(i);
            studentObjects[currentStudent]->drawProjectImage(25, imageHeight + previousHeight, i + 1);
            previousHeight += imageHeight + 30;
        }
        ofPopMatrix();

        // prepare text
        vorName = studentObjects[currentStudent]->first_name;
        nachName = " " + studentObjects[currentStudent]->last_name;
        titel = wrapString(studentObjects[currentStudent]->titel, 450);
        description = wrapString(studentObjects[currentStudent]->description, 450);

        cout << "BUCHSTABEN ae: " << vorName << " oe: " << nachName << " ue: " << description << endl;

        // draw portrait
        ofPushMatrix();
        ofTranslate(infoPanelWidth - 150, infoPanelYPosition + 20);
        ofSetColor(255,255,255);
        ofRect(0,0,studentObjects[currentStudent]->portraitWidth*2.5f +20, studentObjects[currentStudent]->portraitHeight*2.5f+20);
        studentObjects[currentStudent]->image_portrait.draw(10,10, studentObjects[currentStudent]->portraitWidth*2.5f, studentObjects[currentStudent]->portraitHeight*2.5f );

        // draw text
        ofSetColor(10,10,10);
        nameBoxWidth = fontLBold.stringWidth(vorName) + fontLBold.stringWidth(nachName);
        ofSetColor(237,188,0);
        ofRect(-5, studentObjects[currentStudent]->portraitHeight*2.5f + 110, nameBoxWidth + 20, -fontLBold.stringHeight(vorName)-20);

        ofSetColor(255,255,255);
        fontLBold.drawString(vorName,0, studentObjects[currentStudent]->portraitHeight*2.5f + 100);
        fontL.drawString(nachName,fontLBold.stringWidth(vorName), studentObjects[currentStudent]->portraitHeight*2.5f + 100);

        ofSetColor(10,10,10);
        fontSLLight.drawString(ofToUpper(titel),0,studentObjects[currentStudent]->portraitHeight*2.5f + 170);
        fontSLight.drawString(description,0,studentObjects[currentStudent]->portraitHeight*2.5f + 170 + fontS.stringHeight(titel) + 15);

        ofPopMatrix();
        ofPopMatrix();
        break;
        // name view
    case 2:
        if(!zooming)
        {
            for(int i = 0; i < numberOfStudents; i++)
            {
                ofVec3f wordlXYZ = studentObjects[i]->getPosition();
                ofVec3f screenXYZ = camera.worldToScreen(wordlXYZ, ofGetCurrentViewport() );

                ofVec2f screenMid;
                ofVec2f objectPos;

                screenMid.set(ofGetWidth()/2,ofGetHeight()/2);
                objectPos.set(screenXYZ.x, screenXYZ.y);
                // set closest - currentstudent id
                (screenMid.distance(objectPos) < 140) ? studentObjects[i]->setClosestToCamera(true) : studentObjects[i]->setClosestToCamera(false);
                if(studentObjects[i]->getClosestToCamera())
                {
                    closestObjectVector = studentObjects[i]->getPosition();
                    currentStudent = studentObjects[i]->number;
                }
            }
        }

        // draw active student name & short info
        for(int i = 0; i < numberOfStudents; i++)
        {
            if(studentObjects[i]->getClosestToCamera())
            {
//                fullName = studentObjects[i]->getFullName();
                fullName = studentObjects[i]->getFullName();
                vorName = studentObjects[i]->first_name;
                nachName = " " + studentObjects[i]->last_name;

                ofVec3f worldXYZ = studentObjects[i]->getPosition();
                ofVec3f screenXYZ = camera.worldToScreen(worldXYZ, ofGetCurrentViewport());

                int x = screenXYZ.x + 47;
                int y = screenXYZ.y + 10;

                ofSetColor(237,188,0);
                ofRectangle infoRect = fontL.getStringBoundingBox(fullName, 0,0);
                ofRect(x - 5, y - infoRect.height - 5, infoRect.width + 10, infoRect.height +10);
                ofSetColor(255,255,255);
                fontLBold.drawString(vorName, x, y);
                fontL.drawString(nachName, x + fontLBold.stringWidth(vorName), y);
            }
        }
        break;
        // choose currentDepartmentString
    case 3:
        ofEnableAlphaBlending();

        currentDepartmentString = Students::convertFBEnumToString(fachbereichArray[currentDeptNumber]);
        ofSetColor(255,255,255);
        fachbereichTitel.draw(ofGetWidth()/2 - fachbereichTitel.getWidth()/2, 150-fachbereichTitel.getWidth()/2);
        ofSetColor(237,188,0);
        float stringWidth = fontLBold.stringWidth(currentDepartmentString);
        float stringHeight = fontLBold.stringHeight(currentDepartmentString);
        fontLBold.drawString(currentDepartmentString, ofGetWidth()/2 - stringWidth/2, 150 + stringHeight/2);

        ofDisableAlphaBlending();
        break;
    }
}

void WorldView::drawSucher()
{
    ofNoFill();
    ofEnableAlphaBlending();
    ofSetColor(10,10,10,130);
    ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,150, 150);
    ofSetColor(10,10,10,100);
    ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,140, 140);
    ofFill();
}


void WorldView::drawBottomInterface()
{
    ofFill();
    int bottomDebugHeight = 84;
    int inactiveUserIndicatorSize = 15;
    int activeUserIndicatorSize = inactiveUserIndicatorSize * 1.8;

    // draw bottom interface background
    ofEnableAlphaBlending();
    ofSetColor(255,255,255);
    bottomInterface.draw(ofGetWidth()/2 - bottomInterface.getWidth()/2,ofGetHeight()-159);

    // draw indicators outlines
    ofNoFill();
    ofSetColor(237,188,0,255);
    ofEllipse(ofGetWidth()/2, ofGetHeight()- bottomDebugHeight, 64,64);
    ofEllipse(ofGetWidth()/2 + 68, ofGetHeight()- bottomDebugHeight, 26,26);
    ofEllipse(ofGetWidth()/2 - 68, ofGetHeight()- bottomDebugHeight, 26,26);

    // with fill (transparent)
    ofFill();
    ofSetColor(237,188,0, 80);
    ofEllipse(ofGetWidth()/2, ofGetHeight()- bottomDebugHeight, 64,64);
    ofEllipse(ofGetWidth()/2 + 68, ofGetHeight()- bottomDebugHeight, 26,26);
    ofEllipse(ofGetWidth()/2 - 68, ofGetHeight()- bottomDebugHeight, 26,26);

    ofDisableAlphaBlending();
    // active user indicator
    if(cursor->isActiveUser)
    {
        ofSetColor(237,188,0);
        ofEllipse(ofGetWidth()/2, ofGetHeight()- bottomDebugHeight, 64,64);
    }

    // right hand
    if(cursor->rightHandRaised)
    {
        ofSetColor(237,188,0);
        ofEllipse(ofGetWidth()/2 + 68, ofGetHeight()- bottomDebugHeight, 26,26);
    }

    // left hand
    if(cursor->leftHand)
    {
        ofSetColor(237,188,0);
        ofEllipse(ofGetWidth()/2 - 68, ofGetHeight()- bottomDebugHeight, 26,26);
    }

}

void WorldView::keyReleased(ofKeyEventArgs &e)
{
    switch(e.key)
    {
    case 'e':
        changeZoomLevel(ZOOM_IN);
        break;
    case 'q':
        changeZoomLevel(ZOOM_OUT);
        break;
    case 'x':
        changeZoomLevel(ZOOM_RESET);
        break;
    }
}

void WorldView::keyPressed(ofKeyEventArgs &e)
{
    switch(e.key)
    {
    case 'w':
        camera.boom(50.0f);
        break;
    case 's':
        camera.boom(-50.0f);
        break;
    case 'a':
        camera.truck(-50.0f);
        break;
    case 'd':
        camera.truck(50.0f);
        break;
    case 'u':
        swipeGestureEvent(SWIPE_UP);
        break;
    case 'j':
        swipeGestureEvent(SWIPE_DOWN);
        break;
    case 'h':
        swipeGestureEvent(SWIPE_LEFT);
        break;
    case 'k':
        swipeGestureEvent(SWIPE_RIGHT);
        break;

    }
}

void WorldView::changeZoomLevel(zoomLevelEnum _zoomLevel)
{
    static CustomEvent changeZoomLevel;
    changeZoomLevel.zoomLevel = _zoomLevel;
    ofNotifyEvent(CustomEvent::zoomChange, changeZoomLevel);
}

void WorldView::swipeGestureEvent(swipeGesturesEnum _swipeDirection)
{
    static CustomEvent swipeGesture;
    swipeGesture.swipeDirection = _swipeDirection;
    ofNotifyEvent(CustomEvent::swipeGesture, swipeGesture);
}

void WorldView::listenerZoomChange(CustomEvent &e)
{
    menuActive = false;
    justZoomed = true;
    float zoomAmount = cameraHeight+500;

    if(gestureTimerZoom == 0)
    {
        // zoomout
        if(e.zoomLevel == ZOOM_OUT)
        {
            if(zoomLevel == 1)
            {
                zoomLevel++;
                currentStudent = -1;
            }
            else if(zoomLevel < 3)
            {
                zoomLevel++;
                currentCameraHeight +=  zoomAmount;
            }
        }

        // zoomreset, back to beginning
        if(e.zoomLevel == ZOOM_RESET)
        {
            leavingInfoPanelView = true;
        }

        // zoomin
        else if (e.zoomLevel == ZOOM_IN)
        {
            if(zoomLevel > 2)
            {
                zoomLevel--;
                currentCameraHeight -= zoomAmount;
            }
            else if(zoomLevel == 2)
            {
                zoomLevel--;
                justArrived = true;
            }
        }

        // reset gesture timer
        gestureTimerZoom = gestureTimeout;
    }
}

void WorldView::listenerSwipeGesture(CustomEvent &e)
{
    // in overview
    if(zoomLevel == 3 && !departmentChangedToLeft && !departmentChangedToRight && !cursor->leftHand)
    {
        switch(e.swipeDirection)
        {
        case SWIPE_LEFT:
            if(currentDeptNumber == totalDeptNumber)
            {
                currentDeptNumber = -1;
            }
            studentObjectsXPosFuture = -2000;
            departmentChangedToLeft = true;
            break;
        case SWIPE_RIGHT:
            if(currentDeptNumber == 0)
            {
                currentDeptNumber = totalDeptNumber+1;
            }
            studentObjectsXPosFuture = 2000;
            departmentChangedToRight = true;
            break;
        }
    }

    // in info view
    if(zoomLevel == 1 && gestureTimerSwipe == 0)
    {
        switch(e.swipeDirection)
        {
        case SWIPE_LEFT:
            if(currentStudent+1 < numberOfStudents && !infoPanelToRight)
            {
                cout << "swipe left" << endl;
                infoPanelToRight = false;
                infoPanelToLeft = true;
            }
            else shakingTimer = 12;
            break;
        case SWIPE_RIGHT:
            if(currentStudent-1 >= 0 && !infoPanelToLeft)
            {
                cout << "swipe right" << endl;
                infoPanelToLeft = false;
                infoPanelToRight = true;

            }
            else shakingTimer = 12;
            break;
        }
        // reset gesture timer
        gestureTimerSwipe = gestureTimeout;
    }
}

void WorldView::kinectMove()
{
    if(cursor->rightHand)
    {
        moveScreen();
    }
}

void WorldView::moveScreen()
{
    // no scrolling in detail view, move studentObjects
    if(zoomLevel== 2 && !menuActive)
    {
        // X
        camera.truck(currentXDragSpeed);
        // Y, not in overview
        camera.boom(currentYDragSpeed);

    }

    // only move studentObjects left/right
    if(zoomLevel == 3 && !departmentChangedToLeft && !departmentChangedToRight && !menuActive)
    {
        float threshold = ofGetWidth()*0.13;
        studentObjectsXPos -= currentXDragSpeed;
        if(studentObjectsXPos < -threshold) swipeGestureEvent(SWIPE_LEFT);
        if(studentObjectsXPos > threshold) swipeGestureEvent(SWIPE_RIGHT);
    }
}

void WorldView::setCursor(HandCursor *c)
{
    cursor = c;
}


// function to wrap text string into box, input textstring and box width (from c++ forums), returns textstring with added '/n' for formatting
string WorldView::wrapString(string text, int width)
{
    string typeWrapped = "";
    string tempString = "";
    vector <string> words = ofSplitString(text, " ");
    for(int i=0; i<words.size(); i++)
    {
        string wrd = words[i];
        // if we aren't on the first word, add a space
        if (i > 0)
        {
            tempString += " ";
        }
        tempString += wrd;
        int stringwidth = fontS.stringWidth(tempString);
        if(stringwidth >= width)
        {
            typeWrapped += "\n";
            tempString = wrd;       // make sure we're including the extra word on the next line
        }
        else if (i > 0)
        {
            // if we aren't on the first word, add a space
            typeWrapped += " ";
        }
        typeWrapped += wrd;
    }
    return typeWrapped;
}
