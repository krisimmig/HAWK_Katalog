#ifndef MAINMENU_H
#define MAINMENU_H

#include "ofMain.h"
#include "constants.h"
#include "viewchangebutton.h"
#include "button.h"
#include "customevent.h"

class MainMenu
{
    public:
        MainMenu();
        virtual ~MainMenu();

    protected:
    private:
        void draw(ofEventArgs &e);

        RoundButton* buttonGD;
        RoundButton* buttonDM;
        RoundButton* buttonAD;
        ViewChangeButton* buttonToOverview;
        ViewChangeButton* buttonToWorldview;
};

#endif // MAINMENU_H
