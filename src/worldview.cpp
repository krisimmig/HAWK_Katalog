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

    HelveticaXL.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 50);
    HelveticaL.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 26);
    HelveticaS.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 18);
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

    // currentDepartment setup
    fachbereichArray = {FACHBEREICH_ALL,FACHBEREICH_AD,FACHBEREICH_CICD,FACHBEREICH_DM,FACHBEREICH_FG,FACHBEREICH_GD,FACHBEREICH_IAID,FACHBEREICH_LD,FACHBEREICH_MG,FACHBEREICH_PD};
    departmentChangedToLeft = departmentChangedToRight = false;
    totalDeptNumber = 9; // 10 ?!?
    currentDept = 5; // start at GD
    currentDepartment = Students::convertFBEnumToString(fachbereichArray[currentDept]);
    numberOfStudents = Students::countAll(fachbereichArray[currentDept]);
    currentStudent = -1;
    random10 = ofRandom(10);
    firstStart = true;

    // studentObjects
    studentObjectAbstandX = 146;
    studentObjectAbstandY = 174;
    studentObjectsXPos = studentObjectsYPos = studentObjectsXPosDefault = studentObjectsYPosDefault = 0;

    // info panel
    shakingTimer = 0;
    justArrived = false;
    projectImagesYPosition = 0;
    currentProjectImagesYPosition = 0;
    futureProjectImagesYPosition = 0;
    currentImageHeight = 0;
    currentImageNumber = 1;
    infoPanelWidth = 1100;
    infoPanelXPosition = infoPanelFinalXPosition = ofGetWidth()/2 - infoPanelWidth * 0.25;
    infoPanelResetPosition = infoPanelXPosition;
    infoPanelFinalYPosition = 0;
    infoPanelYPosition = 0;
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


    if(firstStart)
    {
        updateDepartment();
        firstStart = false;
    }
    updateStudenObjectsPosition();
    if(zoomLevel > 1) updateScreenPosition();
    if(zoomLevel == 1)
    {
        shakeInfoPanel();
        updateInfoPanelPosition();
    }
    updateZoomLevel();

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
            currentDept++;
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
            currentDept--;
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
    // make new objects
    delete[] studentObjects;
    delete studentIdArray;
    numberOfStudents = Students::countAll(fachbereichArray[currentDept]);
    currentDepartment = Students::convertFBEnumToString(fachbereichArray[currentDept]);

    studentObjects = new Object3D*[numberOfStudents];
    studentIdArray = Students::getStudentIds(fachbereichArray[currentDept]);

    // objects
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
                studentObjects[counter]->setFont(HelveticaXL);
                counter++;
            }
        }
    }
}

void WorldView::updateInfoPanelPosition()
{
    // coming from zoomin
    if(justArrived)
    {
        infoPanelYPosition = 2000;
        justArrived = false;
    }
    cout << "infoPanelYPosition " << ofToString(infoPanelYPosition) << endl;

    if(infoPanelYPosition > 3.0f)
    {
        infoPanelYPosition = infoPanelYPosition * 0.78f;
    }

    // swipe up/down image position update
    float distance = currentProjectImagesYPosition - futureProjectImagesYPosition;

    if(fabs(distance) > 1.5f)
    {
        currentProjectImagesYPosition -= distance * 0.2f;
    }

    // swipe to left infopanel position
    if(infoPanelToLeft && !infoPanelToRight)
    {
        float distance = infoPanelXPosition - infoPanelResetPosition;
        if(infoPanelXPosition > - infoPanelWidth && infoPanelXPosition <= infoPanelResetPosition) infoPanelXPosition -= 150;
        else if(infoPanelXPosition <= - infoPanelWidth )
        {
            infoPanelXPosition = ofGetWidth() + infoPanelWidth;
            currentProjectImagesYPosition = 0;
            futureProjectImagesYPosition = 0;
            currentImageNumber = 1;
            currentStudent++;
        }

        else if(distance > 2.5f) infoPanelXPosition -= distance * 0.2f;
        else
        {
            infoPanelXPosition = infoPanelResetPosition;
            infoPanelToLeft = false;
        }
    }

    // swipe to right infopanel position
    if(infoPanelToRight && !infoPanelToLeft)
    {
        float distance = infoPanelXPosition - infoPanelResetPosition;
        if(distance < 0) distance *= -1;
        if(infoPanelXPosition < ofGetWidth() && infoPanelXPosition >= infoPanelResetPosition) infoPanelXPosition += 150;
        else if(infoPanelXPosition >= ofGetWidth() )
        {
            infoPanelXPosition = -infoPanelWidth;
            currentProjectImagesYPosition = 0;
            futureProjectImagesYPosition = 0;
            currentImageNumber = 1;
            currentStudent--;
        }

        else if(distance > 2.5f) infoPanelXPosition += distance * 0.2f;
        else
        {
            infoPanelXPosition = infoPanelResetPosition;
            infoPanelToRight = false;
        }
    }
}

void WorldView::updateScreenPosition()
{
    // center in overview
    if(zoomLevel == 3)
    {
        if(fabs(camera.getGlobalPosition().x) > 1.0f) camera.truck(-camera.getGlobalPosition().x * 0.1f);
        if(fabs(camera.getGlobalPosition().y) > 1.0f) camera.boom(camera.getGlobalPosition().y * 0.1f);
    }

    // calcualte screen move speed
    if(cursor->cursorDrag && zoomLevel < 3 && !cursor->twoHands)
    {
        // X
        float difference = cursor->smoothRightXPos - pmouseX;
        if(difference > -30 && difference < 30) currentXDragSpeed = -difference * speedFactor;
        // Y
        difference = cursor->smoothRightYPos - pmouseY;
        if(difference > -30 && difference < 30) currentYDragSpeed = -difference  * speedFactor;
    }
    pmouseX = cursor->smoothRightXPos;
    pmouseY = cursor->smoothRightYPos;

    // calculate kinetic scrolling
    // if dragSpeed slower than +-0.5, set to 0, else mulitply by 0.9
    if(!cursor->cursorDrag && zoomLevel > 1 && zoomLevel < 3)
    {
        // X
        if(currentXDragSpeed < -0.5f || currentXDragSpeed > 0.5f)
        {
            currentXDragSpeed *= 0.9f;
            camera.truck(currentXDragSpeed);
        }
        else currentXDragSpeed = 0.0f;
        // Y
        if(currentYDragSpeed < -0.5f || currentYDragSpeed > 0.5f)
        {
            currentYDragSpeed *= 0.9f;
            camera.boom(currentYDragSpeed);
        }
        else currentYDragSpeed = 0.0f;
    }

    // center on closest in zoomlevel 2
    if(zoomLevel == 2 && !cursor->cursorDrag && !zooming)
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
    ofBackgroundGradient(ofColor(255), ofColor(175));
    camera.begin();

    // draw all objects if not detail view
    ofPushMatrix();
    ofTranslate(studentObjectsXPos, studentObjectsYPos);
    for(int i = 0; i < numberOfStudents; i++)
    {
        studentObjects[i]->draw();
    }
    ofPopMatrix();
    kinectMove();

    camera.end();

    drawInfo();
    if(zoomLevel < 3 && zoomLevel > 1) drawSucher();
    drawBottomInterface();

    // draw hands active indicator
    if(cursor->cursorDrag && !cursor->twoHands)
    {
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 200);
        ofEllipse(150, 150, 50, 50);
        ofDisableAlphaBlending();
    }
    else if(cursor->twoHands)
    {
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 200);
        ofEllipse(150, 150, 50, 50);
        ofEllipse(150, 175, 50, 50);
        ofDisableAlphaBlending();
    }
}

void WorldView::shakeInfoPanel()
{
    if(shakingTimer > 0)
    {
        infoPanelYPosition += ofRandom(-15,15);
        shakingTimer--;
    }
    else
    {
        shakingTimer = 0;
    }
}
void WorldView::drawInfo()
{
    int previousHeight = 50;
    int imageHeight = 0;
    int totalImageColumnHeight = 0;
    switch(zoomLevel)
    {
        // student detail view
    case 1:
        // draw opaque background
        ofEnableAlphaBlending();
        ofSetColor(170,170,170, 98);
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        ofDisableAlphaBlending();
        ofPushMatrix();
        ofTranslate(infoPanelXPosition, infoPanelYPosition);

        // draw  left infocards
        ofSetColor(255,255,255);
        ofRect(25,25,350, 850);
        // draw left infocard border
        ofSetColor(170,170,170);
        ofNoFill();
        ofRect(25,25,350, 850);
        ofFill();

        // draw portrait
        studentObjects[currentStudent]->drawPortrait(60,70);

        // draw project images
        ofPushMatrix();
        ofTranslate(0, 0 + currentProjectImagesYPosition);

        // draw  right infocards
        for(int i = 0; i <= studentObjects[currentStudent]->totalNumberProjectImages; i++)
        {
            totalImageColumnHeight += studentObjects[currentStudent]->getProjectImageSize(i) + 30;
        }
        ofSetColor(255,255,255);
        ofRect(475,25,750,totalImageColumnHeight);
        // draw right infocard border
        ofSetColor(170,170,170);
        ofNoFill();
        ofRect(475,25,750,totalImageColumnHeight);
        ofFill();
        for(int i = 0; i < studentObjects[currentStudent]->totalNumberProjectImages; i++)
        {
            if(i > 0) imageHeight = studentObjects[currentStudent]->getProjectImageSize(i);
            studentObjects[currentStudent]->drawProjectImage(500, imageHeight + previousHeight, i + 1);
            previousHeight += imageHeight + 30;
        }
        ofPopMatrix();

        // prepare text
        fullName = studentObjects[currentStudent]->getFullName();
        description = wrapString(studentObjects[currentStudent]->description, 300);
        // draw text
        ofSetColor(10,10,10);
        HelveticaL.drawString(fullName, 60,250);
        HelveticaS.drawString(description, 60, 300);

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
        for(int i = 0; i < numberOfStudents; i++)
        {
            if(studentObjects[i]->getClosestToCamera())
            {
                fullName = studentObjects[i]->getFullName();

                ofVec3f worldXYZ = studentObjects[i]->getPosition();
                ofVec3f screenXYZ = camera.worldToScreen(worldXYZ, ofGetCurrentViewport());

                int x = screenXYZ.x + 47;
                int y = screenXYZ.y + 10;

                ofSetColor(10,10,10);
                ofRectangle infoRect = HelveticaL.getStringBoundingBox(fullName, 0,0);
                ofRect(x - 5, y - infoRect.height - 5, infoRect.width + 10, infoRect.height +10);
                ofSetColor(255,255,255);
                HelveticaL.drawString(fullName, x, y);
            }
        }
        break;
        // choose currentDepartment
    case 3:
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,100,10);
        ofSetColor(255,255,255);
        ofRectangle fachbereichRect = HelveticaL.getStringBoundingBox(currentDepartment,0,0);
        ofEllipse(15,-15, 70,70);

        ofSetColor(10,10,10);
        HelveticaL.drawString(currentDepartment, 0, 0);
        ofPopMatrix();
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
    int bottomDebugHeight = 100;
    int inactiveUserIndicatorSize = 15;
    int activeUserIndicatorSize = inactiveUserIndicatorSize * 1.8;

    // draw bottom interface background
    ofSetColor(255,255,255);
    ofRect(0,ofGetHeight()- bottomDebugHeight, ofGetWidth(), bottomDebugHeight);
    // draw top line
    ofSetColor(170,170,170);
    ofRect(0,ofGetHeight() - bottomDebugHeight, ofGetWidth(), 1);
    // active area top line
    ofSetColor(10,10,10);
    float indicatorWidth = ofGetWidth() * 0.1;
    ofRect(ofGetWidth()/2-indicatorWidth/2,ofGetHeight() - bottomDebugHeight, indicatorWidth, 1);

    // active user indicator
    ofSetColor(255,255,255);
    ofEllipse(ofGetWidth()/2, ofGetHeight()- bottomDebugHeight, 80,80);
    if(cursor->isActiveUser)
    {
        ofSetColor(200,255,255);
        ofEllipse(ofGetWidth()/2, ofGetHeight()- bottomDebugHeight, 75,75);
    }

    // right hand
    ofSetColor(255,255,255);
    ofEllipse(ofGetWidth()/2 + 50, ofGetHeight()- bottomDebugHeight, 50,50);
    if(cursor->cursorDrag)
    {
        ofSetColor(200,255,255);
        ofEllipse(ofGetWidth()/2+ 50, ofGetHeight()- bottomDebugHeight, 45,45);
    }

    // left hand
    ofSetColor(255,255,255);
    ofEllipse(ofGetWidth()/2 - 50,ofGetHeight()-  bottomDebugHeight, 50,50);
    if(cursor->twoHands)
    {
        ofSetColor(200,255,255);
        ofEllipse(ofGetWidth()/2 - 50, ofGetHeight()- bottomDebugHeight, 45,45);
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
            else
            {
                cout << "---- Max Zoomout." << endl;
            }
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
            }
            else
            {
                cout << "---- Max Zoomin." << endl;
            }
        }

        cout << "---- ZoomLevel: " << ofToString(zoomLevel) << endl;

        // reset gesture timer
        gestureTimerZoom = gestureTimeout;
    }

    // change speed to new zoomlevel and activate justarrived bool
    switch(zoomLevel)
    {
    case 1:
        justArrived = true;
        break;
    case 2:
        speedFactor = 0.7f;
        break;
    case 3:
        speedFactor = 1.5f;
        break;
    case 4:
        speedFactor = 1.5f;
        break;
    }
}

void WorldView::listenerSwipeGesture(CustomEvent &e)
{
    // in overview
    if(zoomLevel == 3 && !departmentChangedToLeft && !departmentChangedToRight && !cursor->twoHands)
    {
        switch(e.swipeDirection)
        {
        case SWIPE_LEFT:
            if(currentDept < totalDeptNumber)
            {
                cout << "WorldView::listenerSwipeGesture. LEFT zoomLevel == 3" << endl;
                studentObjectsXPosFuture = -2000;
                departmentChangedToLeft = true;
            }
            else cout << "last dept reached." << endl;
            break;
        case SWIPE_RIGHT:
            if(currentDept > 0)
            {
                cout << "WorldView::listenerSwipeGesture. RIGHT zoomLevel == 3" << endl;
                studentObjectsXPosFuture = 2000;
                departmentChangedToRight = true;
            }
            else cout << "first dept reached." << endl;
            break;
        }
    }

    // in info view
    if(zoomLevel == 1 && gestureTimerSwipe == 0)
    {
        switch(e.swipeDirection)
        {
        case SWIPE_LEFT:
            if(currentStudent < numberOfStudents-1 && !infoPanelToRight)
            {
                infoPanelToRight = false;
                infoPanelToLeft = true;
            }
            else shakingTimer = 12;
            break;
        case SWIPE_RIGHT:
            if(currentStudent > 1 && !infoPanelToLeft)
            {
                infoPanelToLeft = false;
                infoPanelToRight = true;
            }
            else shakingTimer = 12;
            break;
        case SWIPE_UP:
            if(currentImageNumber < studentObjects[currentStudent]->totalNumberProjectImages)
            {
                currentImageHeight = studentObjects[currentStudent]->getProjectImageSize(currentImageNumber);
                futureProjectImagesYPosition -= currentImageHeight + 30;
                currentImageNumber++;
            }
            break;
        case SWIPE_DOWN:
            if(currentImageNumber > 1)
            {
                currentImageHeight = studentObjects[currentStudent]->getProjectImageSize(currentImageNumber - 1);
                futureProjectImagesYPosition += currentImageHeight + 30;
                currentImageNumber--;
            }
            break;
        }

        cout << "cs: " << ofToString(currentStudent)  << "ns: " << ofToString(numberOfStudents) << endl;

        // reset gesture timer
        gestureTimerSwipe = gestureTimeout;
    }
}

void WorldView::kinectMove()
{
    if(cursor->cursorDrag)
    {
        moveScreen(cursor->moveVector);
    }
}

void WorldView::moveScreen(ofVec2f moveVector)
{
    // no scrolling in detail view
    if(zoomLevel > 1 && zoomLevel < 3)
    {
        // X
        camera.truck(currentXDragSpeed);
        // Y
        camera.boom(currentYDragSpeed);
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
        int stringwidth = HelveticaS.stringWidth(tempString);
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



