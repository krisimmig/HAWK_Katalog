#include "students.h"

Students::Students()
{
    //ctor
    cout << "Students ctor" << endl;
}

Students::Students(int id)
{
    //ctor

    setup(id);
}

void Students::setup(int _id)
{
    id = _id;
    sqlite = new ofxSQLite(DB_NAME);
    // -- query students for all first names
    ofxSQLiteSelect dbQuery = sqlite->select("vorname, nachname, fachbereich, titel, beschreibung, image_01")
                              .from("students")
                              .where("id", id);

    dbQuery.execute().begin();

    // process query result, store names
    first_name = dbQuery.getString(0);
    last_name = dbQuery.getString(1);
    fachbereich = dbQuery.getString(2);
    titel = dbQuery.getString(3);
    beschreibung = dbQuery.getString(4);
    image_01 = dbQuery.getString(5);


    // load image
    if(!projectImage01.loadImage(IMAGE_DIR "/" + image_01))
    {
        cout << "error loading image: " << image_01 << endl;
    }

    delete sqlite;

}

void Students::drawInfo(int _x, int _y)
{
    // set position
    x = _x;
    y = _y;

    // bilder rahmen hoehe/breite
    double rahmen = 400;
    int centerXWindow = ofGetWidth() / 2;
    int centerYWindow = ofGetHeight() / 2;

    // draw info
    garamondRegularH1.drawString( first_name + " " + last_name + " (" + fachbereich + ")", 50, 100 );
    garamondRegularH1.drawString( titel, 50, ofGetHeight() - 150 );

    ofDrawBitmapString( first_name + " " + last_name + " (" + fachbereich + ")", 50, 100 );
    ofDrawBitmapString( titel, 50, ofGetHeight() - 150 );

    string beschreibungWrapped = wrapString(beschreibung, 800);
    ofDrawBitmapString(beschreibungWrapped, 50, ofGetHeight() - 125 );

    ofSetLineWidth(2);
    ofLine(0, centerYWindow - rahmen/2 -1, ofGetWidth(), centerYWindow - rahmen/2 -1);
    ofLine(0, centerYWindow + rahmen/2 +1, ofGetWidth(), centerYWindow + rahmen/2 +1);
    ofSetLineWidth(1);

    //draw image01
    // set color to white for tint-free images
    ofSetColor(255,255,255);
    double imageWidth = projectImage01.width;
    double imageHeight = projectImage01.height;
    int ratio = imageWidth / imageHeight;

    if(ratio <= 1)
    {
        // hoeher als breit oder quadratisch
        double conversion = rahmen / imageHeight;
        int resizedImageWidth = imageWidth * conversion;
        int resizedImageHeight = imageHeight * conversion;
        // draw image in center if screen
        projectImage01.draw(centerXWindow - resizedImageWidth/2, centerYWindow - resizedImageHeight/2, resizedImageWidth , resizedImageHeight );
    }
    else
    {
        // breiter als hoch
        double conversion = rahmen / imageWidth;
        int resizedImageWidth = imageWidth * conversion;
        int resizedImageHeight = imageHeight * conversion;
        // draw image in center if screen
        projectImage01.draw(centerXWindow - resizedImageWidth/2, centerYWindow - resizedImageHeight/2, resizedImageWidth , resizedImageHeight );
    }
}

void Students::drawShortInfo(int x, int y)
{
    ofDrawBitmapString( first_name + " " + last_name + " (" + fachbereich + ")", x, y );
    ofDrawBitmapString( titel, x, y + 25 );

    drawImage(x - 200, y - 30, 100);
}

void Students::drawImage(int x, int y, int r)
{
    double rahmen = r;
    //draw image01
    // set color to white for tint-free images
    ofSetColor(255,255,255);
    double imageWidth = projectImage01.width;
    double imageHeight = projectImage01.height;
    int ratio = imageWidth / imageHeight;

    if(ratio <= 1)
    {
        // hoeher als breit oder quadratisch
        double conversion = rahmen / imageHeight;
        int resizedImageWidth = imageWidth * conversion;
        int resizedImageHeight = imageHeight * conversion;
        // draw image in center if screen
        projectImage01.draw(x, y, resizedImageWidth , resizedImageHeight );
    }
    else
    {
        // breiter als hoch
        double conversion = rahmen / imageWidth;
        int resizedImageWidth = imageWidth * conversion;
        int resizedImageHeight = imageHeight * conversion;
        // draw image in center if screen
        projectImage01.draw(x, y, resizedImageWidth , resizedImageHeight );
    }
}

// count number of records in DB
int Students::countAll()
{
    return countAll(FACHBEREICH_ALL);
}

// count number of records in DB
int Students::countAll(fachbereichEnum _fb)
{
    ofxSQLite* sqlite = new ofxSQLite(DB_NAME);
    if(_fb == FACHBEREICH_ALL)
    {
        // count entries
        ofxSQLiteSelect selectCount = sqlite->select("count(*) as total")
                                      .from("students")
                                      .execute().begin();
        delete sqlite;
        return selectCount.getInt();
    }
    else
    {
        // count entries
        std::string fbString = Students::convertFBEnumToString(_fb);
        ofxSQLiteSelect selectCount = sqlite->select("count(*) as total")
                                      .from("students")
                                      .where("fachbereich", fbString)
                                      .execute().begin();
        delete sqlite;
        return selectCount.getInt();
    }
}

int* Students::getStudentIds()
{
    return getStudentIds(FACHBEREICH_ALL);
}

// return array of student IDs
int* Students::getStudentIds(fachbereichEnum _fb)
{

    std::string fb = Students::convertFBEnumToString(_fb);
    ofxSQLite* sqlite = new ofxSQLite(DB_NAME);
    // count entries
    int count;

    if(_fb == FACHBEREICH_ALL)
    {
        count = Students::countAll();
        // make query for all IDs
        ofxSQLiteSelect sel = sqlite->select("id")
                              .from("students")
                              .order("id", " ASC ")
                              .execute().begin();
        delete sqlite;

        // create new int studentIdArray
        int *studentIdArray = new int[count];
        int i = 0;
        // put IDs from SQL query into array
        while(sel.hasNext())
        {
            int id = sel.getInt();
            studentIdArray[i] = id;
            i++;
            sel.next();
        }
        // return int array with IDs
        return studentIdArray;
    }
    else
    {
        count = Students::countAll(_fb);

        // make query for all IDs
        ofxSQLiteSelect sel = sqlite->select("id")
                              .from("students")
                              .where("fachbereich", fb)
                              .order("id", " ASC ")
                              .execute().begin();
        delete sqlite;

        // create new int studentIdArray
        int *studentIdArray = new int[count];
        int i = 0;
        // put IDs from SQL query into array
        while(sel.hasNext())
        {
            int id = sel.getInt();
            studentIdArray[i] = id;
            i++;
            sel.next();
        }
        // return int array with IDs
        return studentIdArray;
    }

}

// function to wrap text string into box, input textstring and box width (from c++ forums), returns textstring with added '/n' for formatting
string Students::wrapString(string text, int width)
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
        int stringwidth = garamondRegularP.stringWidth(tempString);
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

//
std::string Students::convertFBEnumToString(fachbereichEnum _fb)
{
    switch(_fb)
    {
    case FACHBEREICH_ALL:
        return "ALL";
        break;
    case FACHBEREICH_AD:
        return "AD";
        break;
    case FACHBEREICH_CICD:
        return "CICD";
        break;
    case FACHBEREICH_DM:
        return "DM";
        break;
    case FACHBEREICH_FG:
        return "FG";
        break;
    case FACHBEREICH_GD:
        return "GD";
        break;
    case FACHBEREICH_IAID:
        return "IAID";
        break;
    case FACHBEREICH_LD:
        return "LD";
        break;
    case FACHBEREICH_MG:
        return "MG";
        break;
    case FACHBEREICH_PD:
        return "PD";
        break;
    }
}

Students::~Students()
{
    //dtor
    delete sqlite;
}
