#ifndef OVERVIEW_H
#define OVERVIEW_H

#include "ofMain.h"
#include "customevent.h"
#include "button.h"
#include "viewchangebutton.h"
#include "students.h"

class Overview
{
public:
    Overview(std::string currentDept);
    virtual ~Overview();

protected:
private:

    void draw(ofEventArgs &e);
    std::string fachbereich;

    // student info
    int* studentIDs;
    int count;
    ViewChangeButton* buttonToMainmenu;
    ViewChangeButton* buttonToWorldview;
    Students** allStudentsFromDept;
};

#endif // OVERVIEW_H
