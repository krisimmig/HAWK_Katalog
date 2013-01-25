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
    zoomLevel = 3;
    dragSpeed = 30.0f;
    currentDragSpeed = dragSpeed;

    // student setup
    numberOfStudents = 92;

    mySphere = new Object3D*[numberOfStudents];

    // spheres
    sphereSize = 350;
    int abstandX = 156;
    int abstandY = 184;
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

    for(int i = 0; i < numberOfStudents; i++)
    {
        mySphere[i]->setZoomLevel(zoomLevel);
    }

    if(gestureTimer > 0)
    {
        gestureTimer--;
    }
}

void WorldView::setCursor(HandCursor *c)
{
    cursor = c;
}

void WorldView::draw(ofEventArgs &e)
{
    ofBackgroundGradient(ofColor(255), ofColor(175));
    camera.begin();



    // draw all objects
    for(int i = 0; i < numberOfStudents; i++)
    {
        mySphere[i]->draw();
    }

    kinectMove();

    camera.end();
    // overview
    if(zoomLevel == 3)
    {
        ofVec3f middleWordlXYZ = mySphere[numberOfStudents/2]->getPostion();
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
    if(zoomLevel == 1)
    {
        for(int i = 0; i < numberOfStudents; i++)
        {
            ofVec3f wordlXYZ = mySphere[i]->getPostion();
            ofVec3f screenXYZ = camera.worldToScreen(wordlXYZ, ofGetCurrentViewport() );

            ofVec2f screenMid;
            ofVec2f objectPos;

            screenMid.set(ofGetWidth()/2,ofGetHeight()/2);
            objectPos.set(screenXYZ.x, screenXYZ.y);

            (screenMid.distance(objectPos) < 100) ? mySphere[i]->setClosestToCamera(true) : mySphere[i]->setClosestToCamera(false);
        }
    }

    for(int i = 0; i < numberOfStudents; i++)
    {
        if(mySphere[i]->getClosestToCamera())
        {
            std::string fullName = mySphere[i]->getFullName();

            ofVec3f worldXYZ = mySphere[i]->getPostion();
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
    if(zoomLevel < 3)
    {
        ofNoFill();
        ofEnableAlphaBlending();
        ofSetColor(10,10,10,130);
        ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,150, 150);
        ofSetColor(10,10,10,100);
        ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,140, 140);
    }

    // draw hand user tracking indicators
    // hand
    ofFill();
    (cursor->trackingHand) ? ofSetColor(10,255,10,255) : ofSetColor(10,255,10,100);
    ofEllipse(ofGetWidth() - 50, ofGetHeight() - 50, 40, 40);

    // user
    (cursor->trackingUser) ? ofSetColor(10,255,10,255) : ofSetColor(10,255,10,100);
    ofEllipse(ofGetWidth() - 100, ofGetHeight() - 50, 40, 40);
    ofDisableAlphaBlending();

    // calibrating user
    if(cursor->calibratingUser)
    {
        ofSetColor(200,10,10,255);
        ofEllipse(ofGetWidth() - 100, ofGetHeight() - 50, 40, 40);
    }

    // calibrating user
    if(gestureTimer != 0)
    {
        ofSetColor(200,10,10,255);
        ofEllipse(100, ofGetHeight() - 50, 40, 40);
    }

    ofDisableAlphaBlending();

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
        camera.boom(1.0f);
        break;
    case 's':
        camera.boom(-1.0f);
        break;
    case 'a':
        camera.truck(-1.0f);
        break;
    case 'd':
        camera.truck(1.0f);
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
            if(zoomLevel < 3)
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
            if(zoomLevel > 1)
            {
                zoomLevel--;
                currentCameraHeight -= zoomAmount;
            }
            else
            {
                cout << "Max Zoomin." << endl;
            }

        }

        // change camera move speed
        switch(zoomLevel)
        {
        case 1:
            currentDragSpeed = dragSpeed * 0.17f;
            break;
        case 2:
            currentDragSpeed = dragSpeed * 0.45f;
            break;
        case 3:
            currentDragSpeed = dragSpeed;
            break;
        }
        gestureTimer = 30;
    }

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
    // left - right
    if(moveVector.x > pmouseX)
    {
        camera.truck(-currentDragSpeed);
    }
    if(moveVector.x < pmouseX)
    {
        camera.truck(currentDragSpeed);
    }

    // up - down / not in zoomout max
    if(zoomLevel < 3)
    {
        if(moveVector.y > pmouseY)
        {
            camera.boom(currentDragSpeed);
        }
        if(moveVector.y < pmouseY)
        {
            camera.boom(-currentDragSpeed);
        }
    }


    pmouseX = moveVector.x;
    pmouseY = moveVector.y;
}



