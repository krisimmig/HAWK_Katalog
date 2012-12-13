#ifndef SINGLEVIEW_H
#define SINGLEVIEW_H

#include "constants.h"
#include "ofMain.h"
#include "view.h"
#include "students.h"
#include "button.h"
#include "customevent.h"

class Singleview : public View
{
public:
    Singleview();
    virtual void setupButtons();

    void myMouseReleased(ofMouseEventArgs &args);
    void deptChanged(CustomEvent &args);
    void navigate(int x, int y, int b);
    void updateView();
    void updateStudent();
    void draw();

    std::string currentDept;
    int* studentIDs;
    int count;
    int currentPostitionInArray;

    int infoXPos;
    int infoYPos;

    Students* currentStudent;

    // buttons
    RoundButton nextButton;
    RoundButton prevButton;
    RoundButton overviewButton;
    RoundButton mainMenuButton;

    int buttonWidth;
    int buttonHeight;
};

#endif // SINGLEVIEW_H
