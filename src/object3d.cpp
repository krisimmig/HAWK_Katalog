#include "object3d.h"

Object3D::Object3D()
{
//    portraitWidth = 114 * 0.5f;
//    portraitHeight = 146 * 0.5f;

    portraitWidth = 114;
    portraitHeight = 146 ;
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
    replaceChars(first_name = dbQuery.getString(0));

    replaceChars(last_name = dbQuery.getString(1));
    replaceChars(fachbereich = dbQuery.getString(2));
    replaceChars(titel = dbQuery.getString(3));
    replaceChars(description = dbQuery.getString(4));
    replaceChars(image_01 = dbQuery.getString(5));
    replaceChars(file_project_01 = dbQuery.getString(6));
    replaceChars(file_project_02 = dbQuery.getString(7));
    replaceChars(file_project_03 = dbQuery.getString(8));
    replaceChars(file_project_04 = dbQuery.getString(9));

    // count project images
    totalNumberProjectImages = 0;
    if(file_project_01 != "null") totalNumberProjectImages++;
    if(file_project_02 != "null") totalNumberProjectImages++;
    if(file_project_03 != "null") totalNumberProjectImages++;
    if(file_project_04 != "null") totalNumberProjectImages++;


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
}

void Object3D::replaceChars(std::string &inputString)
{
    ofStringReplace(inputString, "Ã¤", "ä");
    ofStringReplace(inputString, "Ã„", "Ä");
    ofStringReplace(inputString, "Ã¶", "ö");
    ofStringReplace(inputString, "Ã–", "Ö");
    ofStringReplace(inputString, "Ã¼", "ü");
    ofStringReplace(inputString, "Ãœ", "Ü");
    ofStringReplace(inputString, "ÃŸ", "ß");
    ofStringReplace(inputString, "â€“", "-");
    ofStringReplace(inputString, "â€ž", "\"");
    ofStringReplace(inputString, "â€?", "\"");
    ofStringReplace(inputString, "â€œ", "\"");
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

void Object3D::draw()
{
    float rahmen = 6;
    if(zoomLevel > 2) closestToCamera = false;

    // highlight portrait
    if(closestToCamera)
    {
        ofSetColor(237,188,0);
        int rahmen = 10;
        ofRect(x-rahmen,y-rahmen,z,portraitWidth+rahmen*2, portraitHeight+rahmen*2);
    }
    // draw image
    ofSetColor(255,255,255);

    ofRect(x-rahmen,y-rahmen,z,portraitWidth + rahmen*2, portraitHeight + rahmen*2);
    image_portrait.draw(x,y,z, portraitWidth, portraitHeight);
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
    image_portrait.draw(portraitPosX,portraitPosY,z, portraitWidth, portraitHeight);
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
    pos.set(x + portraitWidth/2, y + portraitHeight/2 , z);
    return pos;
}
