#include "object3d.h"

Object3D::Object3D()
{

}
Object3D::~Object3D()
{

}

void Object3D::setup(int _x, int _y, int _z, int _number, int _id)
{
    // general
    closestToCamera = false;
    number = _number;
    id = _id;
    x = _x;
    y = _y;
    z = _z;

    id = _id;

    // make DB QUERY
    sqlite = new ofxSQLite(DB_NAME);
    // -- query students for all first names
    ofxSQLiteSelect dbQuery = sqlite->select("vorname, nachname, fachbereich, titel, beschreibung, image_01, image_02, image_03, image_04, image_05")
                              .from("students")
                              .where("id", id);

    dbQuery.execute().begin();

    // process query result, store names
    first_name = dbQuery.getString(0);
    last_name = dbQuery.getString(1);
    fachbereich = dbQuery.getString(2);
    titel = dbQuery.getString(3);
    description = dbQuery.getString(4);
    image_01 = dbQuery.getString(5);
    file_project_01 = dbQuery.getString(6);
    file_project_02 = dbQuery.getString(7);
    file_project_03 = dbQuery.getString(8);
    file_project_04 = dbQuery.getString(9);

    fullName = first_name + " " + last_name;
    // load image
    if(!image_portrait.loadImage(PORTRAITS_DIR "/" + image_01))
    {
        cout << "error loading image: " << image_01 << endl;
    }
    delete sqlite;

    zoomLevel = 4;

    isMaster = false;
    projectImagesLoaded = false;
    totalNumberProjectImages = ofRandom(3, 4);
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
    ofSetColor(10,10,10);
    ofDrawBitmapString(ofToString(number), x + 20 , y + 20);
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
    ofSetColor(10,10,10);
    ofDrawBitmapString(ofToString(number), portraitPosX + 20 , portraitPosY + 20);
}

std::string Object3D::getFullName()
{
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
