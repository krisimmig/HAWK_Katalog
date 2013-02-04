#include "object3d.h"

Object3D::Object3D()
{

}
Object3D::~Object3D()
{

}

void Object3D::setup(int _x, int _y, int _z, int _size, int _id)
{
    // general
    closestToCamera = false;
    id = _id;
    x = _x;
    y = _y;
    z = _z;
    size = 50;

    letters=ofSplitString("a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z", ",");
    letter = ofToString(letters[ofRandom(0,25)]);
    description = "Käferkunde ist eine App, mit der Käfer gelernt, nachgeschlagen,bestimmt oder gezeigt werden können. Allein in Deutschland gibt es ca. 6.500 verschiedene Käferarten. Während wir sie kaum beachten, müssen Förster sie kennen und lernen. Mit der Käferkunde-App ist das Lernen/Nachschlagen von Käfern jetzt schnell und einfach möglich. Daneben können gesichtete Käfer auf einer interaktiven Karte mit Foto eingezeichnet oder mit Hilfe der Filterfunktion schnell bestimmt werden. kaeferkunde.fovea.eu";

    int number = ofRandom(1,5);
    switch(number)
    {
    case 1:
        fullName = "Katrin Mahler";
        break;
    case 2:
        fullName = "Stefanie Ernst";
        break;
    case 3:
        fullName = "Melanie Stahl";
        break;
    case 4:
        fullName = "Katharina Eberts";
        break;
    case 5:
        fullName = "Claudia Blau";
        break;
    }

    zoomLevel = 4;

    (ofRandom(10) > 7) ? isABC = true : isMaster = false;
    (ofRandom(10) > 8) ? isMaster = true : isMaster = false;

    int portraitNumber = ofRandom(1, 5);
    totalNumberProjectImages = ofRandom(3, 4);

    file_portrait = ofToString(portraitNumber) + ".jpg";

    int one = ofRandom(1, 10);
    int two = ofRandom(1, 10);
    int three = ofRandom(1, 10);
    int four = ofRandom(1, 10);
    file_project_01 = ofToString(one) + ".jpg";
    file_project_02 = ofToString(two) + ".jpg";
    file_project_03 = ofToString(three) + ".jpg";
    file_project_04 = ofToString(four) + ".jpg";

    // load image
    image_portrait.loadImage(PORTRAITS_DIR "/" + file_portrait);

    projectImagesLoaded = false;
}

void Object3D::loadProjectImages()
{
    image_project_01.loadImage(IMAGE_DIR "/" + file_project_01);
    image_project_02.loadImage(IMAGE_DIR "/" + file_project_02);
    image_project_03.loadImage(IMAGE_DIR "/" + file_project_03);
    image_project_04.loadImage(IMAGE_DIR "/" + file_project_04);
    projectImagesLoaded = true;
}

int Object3D::getProjectImageSize(int imageNumber)
{
    if(imageNumber <= totalNumberProjectImages)
    {
        switch(imageNumber)
        {
        case 1:
            return image_project_01.getHeight();
        case 2:
            return image_project_02.getHeight();
        case 3:
            return image_project_03.getHeight();
        case 4:
            return image_project_04.getHeight();
        }
    }
}

void Object3D::draw()
{
    float rahmen = 6;
    if(zoomLevel > 2) closestToCamera = false;

    // highlight portrait
    if(closestToCamera)
    {
        ofSetColor(10,10,10);
        ofRect(x-5,y-5,z,image_portrait.getWidth()+10, image_portrait.getHeight()+10);
    }
    // draw image
    ofSetColor(255,255,255);

    ofRect(x-rahmen,y-rahmen,z,image_portrait.getWidth() + rahmen*2, image_portrait.getHeight() + rahmen*2);
    image_portrait.draw(x,y,z);

    // master marker
    if(isMaster && zoomLevel < 4)
    {
        ofSetColor(200,200,0);
        ofRect(x+image_portrait.getWidth() * 0.9,y-rahmen,z,image_portrait.getWidth() * 0.12, image_portrait.getHeight()*0.25);
    }

    if(isABC && zoomLevel == 3)
    {
        ofEnableAlphaBlending();
        ofSetColor(10,10,10,180);
        ofRect(x-5,y-5,z,image_portrait.getWidth()+10, image_portrait.getHeight()+10);
        ofDisableAlphaBlending();
        ofSetColor(255,255,255);
        font1->drawString(ofToUpper(letter), x + 15,y + 70);
    }
}

void Object3D::drawProjectImage(int _x, int _y, int _imageNumber)
{
    int projectImagePosX = _x;
    int projectImagePosY = _y;
    int projectImageNumber = _imageNumber;

    if(!projectImagesLoaded) loadProjectImages();

    // draw image
    ofSetColor(255,255,255);

    switch(projectImageNumber)
    {
    case 1:
        image_project_01.draw(projectImagePosX,projectImagePosY,z);
        break;
    case 2:
        image_project_02.draw(projectImagePosX,projectImagePosY,z);
        break;
    case 3:
        image_project_03.draw(projectImagePosX,projectImagePosY,z);
        break;
    case 4:
        image_project_04.draw(projectImagePosX,projectImagePosY,z);
        break;
    }
}


void Object3D::drawPortrait(int _x, int _y)
{
    int portraitPosX = _x;
    int portraitPosY = _y;
    // draw image
    ofSetColor(255,255,255);
    image_portrait.draw(portraitPosX,portraitPosY,z);
}

std::string Object3D::getFullName()
{
//    std::string fullName = first_name + " " + last_name;
    return fullName;
}

void Object3D::setFont(ofTrueTypeFont &_font)
{
    font1 = &_font;
}

void Object3D::setZoomLevel(int _zoomLevel)
{
    zoomLevel = _zoomLevel;
}

void Object3D::setClosestToCamera(bool _closest)
{
    closestToCamera = _closest;
}

bool Object3D::getClosestToCamera()
{
    return closestToCamera;
}

ofVec3f Object3D::getPosition()
{
    ofVec3f pos;
    pos.set(x + image_portrait.getWidth()/2, y + image_portrait.getHeight()/2 , z);
    return pos;
}
