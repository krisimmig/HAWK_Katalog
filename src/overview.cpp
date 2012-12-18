#include "overview.h"

Overview::Overview(std::string cd)
{
    //ctor
    ofBackground(200,200,200);
    fachbereich = cd;
    ofAddListener(ofEvents().draw, this, &Overview::draw);
    buttonToMainmenu = new ViewChangeButton("Main", MAINMENU, 800, 150);
    buttonToWorldview = new ViewChangeButton("Worldview", WORLDVIEW, 800, 200);

    // get array of all student IDs from fachbereich
    studentIDs = Students::getStudentIds(fachbereich);
    count = Students::countAll(fachbereich);
    allStudentsFromDept = new Students*[count];

    for (int i = 0; i < count; i++)
    {
        allStudentsFromDept[i] = new Students(studentIDs[i]);
    }
}

Overview::~Overview()
{
    delete buttonToMainmenu;
    delete buttonToWorldview;
    delete[] allStudentsFromDept;
    ofRemoveListener(ofEvents().draw, this, &Overview::draw);
}

void Overview::draw(ofEventArgs &e)
{
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("Overview", 300, 50);
    ofDrawBitmapString("Fachbereich: " + fachbereich, 300, 75);
    ofDrawBitmapString("Anzahl: " + ofToString(count), 300, 100);

    int abstand = 60;
    int y = 350;
    for (int i = 0; i < count; i++)
    {
        allStudentsFromDept[i]->drawShortInfo(300, y);
        y += abstand;
    }
}
