#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyReleased, this, &WorldView::keyReleased);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofAddListener(CustomEvent().zoomChange, this, &WorldView::zoomChangeListener);
    ofAddListener(CustomEvent().swipeGesture, this, &WorldView::swipeGestureListener);

    HelveticaXL.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 50);
    HelveticaL.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 26);
    HelveticaS.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 18);
    ofEnableSmoothing();

    // gestures
    gestureTimerZoom = 0;
    gestureTimerSwipe = 0;
    gestureTimeout = 30;

    // camera
    cameraHeight = -2500;
    ofVec3f v1;
    ofVec3f v2;
    v1.set(0,1,0);
    v2.set(0,0,1);
    camera.rotate(180, v1);
    camera.rotate(180, v2);
    currentCameraHeight = cameraHeight;
    camera.setPosition(1000,1000,currentCameraHeight);
    zoomLevel = 4;
    dragSpeed = 30.0f;
    currentYDragSpeed = dragSpeed;
    speedXCounter = 0;
    speedYCounter = 0;

    // student setup
    numberOfStudents = 92;
    currentStudent = -1;
    random10 = ofRandom(10);

    // info panel
    justArrived = false;
    projectImagesYPosition = 0;
    currentProjectImagesYPosition = 0;
    futureProjectImagesYPosition = 0;
    currentImageHeight = 0;
    currentImageNumber = 1;
    infoPanelWidth = 1100;
    infoPanelXPosition = ofGetWidth()/2 - infoPanelWidth * 0.25;
    infoPanelResetPosition = infoPanelXPosition;
    infoPanelFinalYPosition = 0;
    infoPanelYPosition = 0;
    mySphere = new Object3D*[numberOfStudents];

    // spheres
    sphereSize = 350;
    int abstandX = 146;
    int abstandY = 174;
    int counter = 0;
    for(int i = 0; i < numberOfStudents; i++)
    {
        for(int j = 0; j < sqrt(numberOfStudents); j++)
//        for(int j = 0; j < 17; j++)
        {
            if(counter < numberOfStudents)
            {
                int x = abstandX * i;
                int y = abstandY * j;
                int z = 0;
                mySphere[counter] = new Object3D();
                mySphere[counter]->setup(x,y,z, sphereSize, counter);
                mySphere[counter]->setFont(HelveticaXL);
                counter++;
            }
        }
    }
    ofFill();
}

WorldView::~WorldView()
{
    //dtor
    ofRemoveListener(ofEvents().draw, this, &WorldView::draw);
    ofRemoveListener(ofEvents().update, this, &WorldView::update);
    ofRemoveListener(ofEvents().keyReleased, this, &WorldView::keyReleased);
    delete mySphere;
}

void WorldView::update(ofEventArgs &e)
{
    if(zoomLevel > 1) updateScreenPosition();
    if(zoomLevel == 1) updateInfoPanelPosition();
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

void WorldView::updateInfoPanelPosition()
{
    // coming from zoomin
    if(justArrived)
    {
        infoPanelYPosition = ofGetHeight();
        justArrived = false;
    }

    if(infoPanelYPosition > infoPanelFinalYPosition)
    {
        infoPanelYPosition -= (infoPanelFinalYPosition + infoPanelYPosition) * 0.25f;
    }
    else
    {
        infoPanelYPosition = infoPanelFinalYPosition;
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
    // calcualte screen move speed
    if(cursor->cursorDrag)
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
    if(!cursor->cursorDrag && zoomLevel > 1)
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
        camera.dolly(40);

    }
    // zoomin
    else if(camera.getPosition().z < currentCameraHeight)
    {
        zooming = true;
        camera.dolly(-40);
    }
    else zooming = false;

    // set zoomlevel for obejcts
    for(int i = 0; i < numberOfStudents; i++)
    {
        mySphere[i]->setZoomLevel(zoomLevel);
    }
}

void WorldView::draw(ofEventArgs &e)
{
    ofFill();
    ofBackgroundGradient(ofColor(255), ofColor(175));
    camera.begin();

    // draw all objects if not detail view
    for(int i = 0; i < numberOfStudents; i++)
    {
        mySphere[i]->draw();
    }

    kinectMove();

    camera.end();

    drawInfo();
    if(zoomLevel < 4 && zoomLevel > 1) drawSucher();
    drawBottomInterface();
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
        mySphere[currentStudent]->drawPortrait(60,70);

        // draw project images
        ofPushMatrix();
        ofTranslate(0, 0 + currentProjectImagesYPosition);

        // draw  right infocards
        for(int i = 0; i <= mySphere[currentStudent]->totalNumberProjectImages; i++)
        {
            totalImageColumnHeight += mySphere[currentStudent]->getProjectImageSize(i) + 30;
        }
        ofSetColor(255,255,255);
        ofRect(475,25,750,totalImageColumnHeight);
        // draw right infocard border
        ofSetColor(170,170,170);
        ofNoFill();
        ofRect(475,25,750,totalImageColumnHeight);
        ofFill();
        for(int i = 0; i < mySphere[currentStudent]->totalNumberProjectImages; i++)
        {
            if(i > 0) imageHeight = mySphere[currentStudent]->getProjectImageSize(i);
            mySphere[currentStudent]->drawProjectImage(500, imageHeight + previousHeight, i + 1);
            previousHeight += imageHeight + 30;
        }

        ofPopMatrix();

        // prepare text
        fullName = mySphere[currentStudent]->getFullName();
        description = wrapString(mySphere[currentStudent]->description, 300);
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
                ofVec3f wordlXYZ = mySphere[i]->getPosition();
                ofVec3f screenXYZ = camera.worldToScreen(wordlXYZ, ofGetCurrentViewport() );

                ofVec2f screenMid;
                ofVec2f objectPos;

                screenMid.set(ofGetWidth()/2,ofGetHeight()/2);
                objectPos.set(screenXYZ.x, screenXYZ.y);
                // set closest - currentstudent id
                (screenMid.distance(objectPos) < 140) ? mySphere[i]->setClosestToCamera(true) : mySphere[i]->setClosestToCamera(false);
                if(mySphere[i]->getClosestToCamera())
                {
                    closestObjectVector = mySphere[i]->getPosition();
                    currentStudent = i;
                }
            }
        }
        for(int i = 0; i < numberOfStudents; i++)
        {
            if(mySphere[i]->getClosestToCamera())
            {
                fullName = mySphere[i]->getFullName();

                ofVec3f worldXYZ = mySphere[i]->getPosition();
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
        // alphabet view
    case 3:
        break;
        // choose fachbereich
    case 4:
        ofVec3f middleWordlXYZ = mySphere[numberOfStudents/2]->getPosition();
        ofVec3f middleScreenXYZ = camera.worldToScreen(middleWordlXYZ, ofGetCurrentViewport() );

        std::string fachbereich = "GRAFIKDESIGN";
        ofSetColor(10,10,10);
        ofRectangle fachbereichRect = HelveticaL.getStringBoundingBox(fachbereich, 0,0);
        int x = middleScreenXYZ.x - fachbereichRect.width / 2;
        int y = middleScreenXYZ.y + fachbereichRect.height / 2;
        ofPushMatrix();
        ofTranslate(0,0,50);
        ofRect(x - 5, y - fachbereichRect.height - 5, fachbereichRect.width + 10, fachbereichRect.height +10);
        ofSetColor(255,255,255);

        HelveticaL.drawString(fachbereich, x, y);
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

    // sucher active drag
    if(cursor->cursorDrag)
    {
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 100);
        ofEllipse(ofGetWidth() / 2, ofGetHeight() / 2, 140, 140);
        ofDisableAlphaBlending();
    }

    ofFill();
}

void WorldView::drawBottomInterface()
{
    ofFill();
    int bottomDebugHeight = 45;
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

    // draw inactiveUser indicators
    for(int i = 0; i < 6; i++)
    {
        float userXPos = cursor->usersPos[i];
        if(userXPos != 0.0f)
        {
            float newUserXPos = userXPos + 1200;
            float pos = (ofGetWidth()/100) * (newUserXPos / 24);
            ofSetColor(100,100,100);
            ofEllipse(pos, ofGetHeight() - bottomDebugHeight, inactiveUserIndicatorSize, inactiveUserIndicatorSize );
        }
    }

    // draw activeUser indicator
    if(cursor->isActiveUser && cursor->activeUserPos != 0.0f)
    {
        ofSetColor(10,10,10);
        // draw active User indicator
        float activeUserXPos = cursor->activeUserPos + 1200;
        float pos = (ofGetWidth()/100) * (activeUserXPos / 24);
        ofNoFill();
        ofEllipse(pos, ofGetHeight() - bottomDebugHeight, activeUserIndicatorSize, activeUserIndicatorSize );
        ofFill();
        ofSetColor(255,255,255);
        ofEllipse(pos, ofGetHeight() - bottomDebugHeight, activeUserIndicatorSize, activeUserIndicatorSize );
    }

    // draw gesture indicators
    float widthProgressBar = 100.0f;
    float heightProgressBar = 6;
    float gestureIndicatorYPos = ofGetHeight() - bottomDebugHeight/2 - heightProgressBar/2;
    int distanceToMiddle = 35;
    // draw boxes
    ofSetColor(200,200,200);
    ofRect(ofGetWidth()/2 - widthProgressBar - distanceToMiddle,gestureIndicatorYPos,widthProgressBar,heightProgressBar);
    ofRect(ofGetWidth()/2 + distanceToMiddle,gestureIndicatorYPos,widthProgressBar,heightProgressBar);

    // draw progress
    ofSetColor(10,10,10);
    if(cursor->zoomInGestureTimer > 0)
    {
        float progressIn = (cursor->zoomInGestureTimer - 1) / (cursor->zoomGestureDuration / 100.0f);
        ofRect(ofGetWidth()/2 + 20,gestureIndicatorYPos,(widthProgressBar/100) * progressIn, heightProgressBar);
    }
    if(cursor->zoomOutGestureTimer > 0)
    {
        float progressOut = (cursor->zoomOutGestureTimer - 1) / (cursor->zoomGestureDuration / 100.0f);
        ofRect(ofGetWidth()/2 - 20,gestureIndicatorYPos,(widthProgressBar/100) * -progressOut, heightProgressBar);
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

void WorldView::zoomChangeListener(CustomEvent &e)
{

    float zoomAmount = (cameraHeight+500)/2;

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
            else if(zoomLevel < 4)
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

void WorldView::swipeGestureListener(CustomEvent &e)
{

    if(zoomLevel == 1 && gestureTimerSwipe == 0)
    {
        switch(e.swipeDirection)
        {

        case SWIPE_LEFT:
            if(currentStudent < numberOfStudents && !infoPanelToRight)
            {
                infoPanelToRight = false;
                infoPanelToLeft = true;
            }
            else cout << "studentlist end reached." << endl;
            break;
        case SWIPE_RIGHT:
            if(currentStudent > 0 && !infoPanelToLeft)
            {
                infoPanelToLeft = false;
                infoPanelToRight = true;
            }
            else cout << "studentlist beginning reached." << endl;
            break;
        case SWIPE_UP:
            cout << "----------------------------------up" << endl;
            if(currentImageNumber < mySphere[currentStudent]->totalNumberProjectImages)
            {
                currentImageHeight = mySphere[currentStudent]->getProjectImageSize(currentImageNumber);
                futureProjectImagesYPosition -= currentImageHeight + 30;
                currentImageNumber++;
            }
            break;
        case SWIPE_DOWN:
            cout << "*********************************down" << endl;
            if(currentImageNumber > 1)
            {
                currentImageHeight = mySphere[currentStudent]->getProjectImageSize(currentImageNumber - 1);
                futureProjectImagesYPosition += currentImageHeight + 30;
                currentImageNumber--;
            }
            break;
        }

        cout << "cs: " << ofToString(currentStudent) << endl;

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
    if(zoomLevel > 1)
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



