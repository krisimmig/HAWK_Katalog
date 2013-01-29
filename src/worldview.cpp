#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyReleased, this, &WorldView::keyReleased);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofAddListener(ofEvents().mouseDragged, this, &WorldView::mouseDragged);
    ofAddListener(CustomEvent().zoomChange, this, &WorldView::zoomChangeListener);

    HelveticaXL.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 50);
    HelveticaL.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 30);
    HelveticaS.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 15);
    ofEnableSmoothing();
    gestureTimer = 0;
    random10 = ofRandom(10);

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
    switch(zoomLevel)
    {
    case 2:
        speedFactor = 0.5f;
        break;
    case 3:
        speedFactor = 1.5f;
        break;
    case 4:
        speedFactor = 1.5f;
        break;
    }

    // zoomout
    if(camera.getPosition().z > currentCameraHeight)
    {
        camera.dolly(40);
    }
    // zoomin
    if(camera.getPosition().z < currentCameraHeight)
    {
        camera.dolly(-40);
    }

    // set zoomlevel for obejcts
    for(int i = 0; i < numberOfStudents; i++)
    {
        mySphere[i]->setZoomLevel(zoomLevel);
    }

    // calcualte screen move speed
    // X
    if(cursor->cursorDrag)
    {
        float difference = cursor->smoothXPos - pmouseX;
        if(difference > -30 && difference < 30) currentXDragSpeed = -difference * speedFactor;
        if(speedXCounter = 10)
        {
            pmouseX = cursor->smoothXPos;
            speedXCounter = 0;
        }
        else speedXCounter++;

        // Y
        difference = cursor->smoothYPos - pmouseY;
        if(difference > -30 && difference < 30) currentYDragSpeed = -difference  * speedFactor;
        if(speedYCounter = 10)
        {
            pmouseY = cursor->smoothYPos;
            speedYCounter = 0.0f;
        }
        else speedYCounter++;
    }

    // calculate kinetic scrolling
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
    if(zoomLevel == 2 && !cursor->cursorDrag)
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

    // gesture timer
    if(gestureTimer > 0)
    {
        gestureTimer--;
    }
}

void WorldView::draw(ofEventArgs &e)
{
    ofFill();
    ofBackgroundGradient(ofColor(255), ofColor(175));
    camera.begin();

    // draw all objects
    for(int i = 0; i < numberOfStudents; i++)
    {
        mySphere[i]->draw();
    }

    kinectMove();

    camera.end();
    // overview -- draw fachbereich string
    if(zoomLevel == 4)
    {
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
    }

    // toggle center object highlight
    if(zoomLevel == 2)
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

    // draw student detail info
    if(zoomLevel == 1)
    {
        ofEnableAlphaBlending();
        ofSetColor(10,10,10,100);
        ofRect(0,0,ofGetWidth(), ofGetHeight());
        ofDisableAlphaBlending();
        std::string fullName = mySphere[currentStudent]->getFullName();
        ofSetColor(255,255,255);
        HelveticaXL.drawString(fullName, 100,100);
    }

    for(int i = 0; i < numberOfStudents; i++)
    {
        if(mySphere[i]->getClosestToCamera())
        {
            std::string fullName = mySphere[i]->getFullName();

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

    // draw sucher
    if(zoomLevel < 4)
    {
        ofNoFill();
        ofEnableAlphaBlending();
        ofSetColor(10,10,10,130);
        ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,150, 150);
        ofSetColor(10,10,10,100);
        ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,140, 140);
    }

    drawDebug();
}

void WorldView::drawDebug()
{
    // draw hand user tracking indicators
    // hand
    if(cursor->cursorDrag)
    {
        ofFill();
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 100);
        ofEllipse(ofGetWidth() / 2, ofGetHeight() / 2, 140, 140);
        ofDisableAlphaBlending();
    }


    // user
    (cursor->trackingUser) ? ofFill() : ofNoFill();
    ofEllipse(ofGetWidth() - 100, ofGetHeight() - 50, 40, 40);

    // calibrating user
    if(cursor->calibratingUser)
    {
        ofSetColor(200,10,10);
        ofEllipse(ofGetWidth() - 100, ofGetHeight() - 50, 40, 40);
    }

    // gesture timer
    ofSetColor(200,10,10);
    (gestureTimer != 0) ? ofFill() : ofNoFill();
    ofEllipse(100, ofGetHeight() - 50, 40, 40);
    ofFill();
}

void WorldView::keyReleased(ofKeyEventArgs &e)
{
    switch(e.key)
    {
    case 'e':
        changeZoomLevel(1);
        break;
    case 'q':
        changeZoomLevel(0);
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
    }
}

void WorldView::changeZoomLevel(int _zoomLevel)
{
    static CustomEvent changeZoomLevel;
    changeZoomLevel.zoomLevel = _zoomLevel;
    ofNotifyEvent(CustomEvent::zoomChange, changeZoomLevel);
}

void WorldView::zoomChangeListener(CustomEvent &e)
{
    float zoomAmount = (cameraHeight+500)/2;

    if(gestureTimer == 0)
    {
        // 0 = zoomout
        if(e.zoomLevel == 0)
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
                cout << "Max Zoomout." << endl;
            }
        }

        // 1 = zoomin
        else if (e.zoomLevel == 1)
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
                cout << "Max Zoomin." << endl;
            }
        }


        // reset geture timer
        gestureTimer = 30;
    }
    cout << "ZoomLevel: " << ofToString(zoomLevel) << endl;
}

void WorldView::mouseDragged(ofMouseEventArgs &e)
{
    ofVec2f mouseCoords;
    mouseCoords.set(e.x, e.y);
    moveScreen(mouseCoords);
}
//
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



