#include "mainmenu.h"

MainMenu::MainMenu()
{
    //ctor
    ofBackground(200,200,200);
    ofAddListener(ofEvents().draw, this, &MainMenu::draw);
//    buttonGD = new RoundButton("GD", "GD",  250, 250);
//    buttonDM = new RoundButton("DM", "DM", 350, 250);
//    buttonAD = new RoundButton("AD", "AD", 450, 250);
//    buttonToOverview = new ViewChangeButton("Overview", OVERVIEW, 800, 150);
    buttonToWorldview = new ViewChangeButton("START", WORLDVIEW, 800, 200);
}

MainMenu::~MainMenu()
{
    //dtor
    ofRemoveListener(ofEvents().draw, this, &MainMenu::draw);
//    delete buttonGD;
//    delete buttonDM;
//    delete buttonAD;
//    delete buttonToOverview;
    delete buttonToWorldview;
}

void MainMenu::draw(ofEventArgs &e)
{
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("Mainview", 300, 50);
//    buttonToWorldview->changeView(buttonToWorldview->targetView);
}
