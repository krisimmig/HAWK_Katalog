#include "singleview.h"

Singleview::Singleview()
{
    // start in student list on position 0
    currentPostitionInArray = 0;
    //studen infos position
    infoXPos = 250;
    infoYPos = 100;
    ofAddListener(CustomEvent::deptChange, this, &Singleview::deptChanged);
    currentStudent = new Students();
}

void Singleview::updateView()
{
    // get array of all student IDs
    studentIDs = Students::getStudentIds(currentDept);
    count = Students::countAll(currentDept);
    currentPostitionInArray = 0;
    updateStudent();
}

void Singleview::setupButtons()
{
    // ---------------------------------
    // buttons setup
    // ---------------------------------
    nextButton.setup("Next");
    prevButton.setup("Prev");
    mainMenuButton.setup("Main");
    overviewButton.setup("Overview");
}

void Singleview::draw()
{
    // draw buttons
    mainMenuButton.draw(ofGetWidth() - 100 , 80);
    overviewButton.draw(ofGetWidth() - 100 , 240);
    nextButton.draw(ofGetWidth() - 100 , 370);
    prevButton.draw(ofGetWidth() - 100 , 470);

    //draw student project info
    currentStudent->drawInfo(infoXPos, infoYPos);

    // if user hand is tracked
    if(trackingHand)
    {
        // ask button if cursor is hoovering
        if(overviewButton.hoverState(cursor->xPos, cursor->yPos))
        {
            viewmanager->setView(OVERVIEW);
        }

        if(mainMenuButton.hoverState(cursor->xPos, cursor->yPos))
        {
            viewmanager->setView(MAINMENU);
        }

        // single navigation
        if(nextButton.hoverState(cursor->xPos, cursor->yPos))
        {
            //if cursor timer is up, call naigate with cursor x&y position
            navigate(cursor->xPos, cursor->yPos, 0);
        }
        if(prevButton.hoverState(cursor->xPos, cursor->yPos))
        {
            navigate(cursor->xPos, cursor->yPos, 0);
        }
    }
}

void Singleview::myMouseReleased(ofMouseEventArgs &args)
{
    navigate(args.x, args.y, 0);
}

void Singleview::deptChanged(CustomEvent &args)
{
    cout << "custom event received: " << args.dept << endl;
    currentDept = args.dept;
    updateView();
}

void Singleview::navigate(int x, int y, int button)
{
    //check buttons with x&y coordinates to determine which button was pressed

    if(nextButton.mouseReleased(x, y, button))
    {
        if(currentPostitionInArray < count - 1)
        {
            currentPostitionInArray++;
            updateStudent();
        }
    }

    if(prevButton.mouseReleased(x, y, button))
    {
        if(currentPostitionInArray > 0)
        {
            currentPostitionInArray--;
            updateStudent();
        }
    }

    if(overviewButton.mouseReleased(x, y, button))
    {
        viewmanager->setView(OVERVIEW);
    }

    if(mainMenuButton.mouseReleased(x, y, button))
    {
        viewmanager->setView(MAINMENU);
    }
}

void Singleview::updateStudent()
{
    currentStudent->setup(studentIDs[currentPostitionInArray]);
}
