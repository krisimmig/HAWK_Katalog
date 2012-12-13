#ifndef MAINMENU_H
#define MAINMENU_H
#include "ofMain.h"
#include "constants.h"
#include "button.h"
#include "view.h"
#include "singleview.h"
#include "viewmanager.h"
#include "customevent.h"


class MainMenu : public View
{
    public:
        MainMenu();
        virtual ~MainMenu();

        virtual void setupButtons();
        void draw();
        void deptChangeEvent(std::string d);
        void mouseReleased(int x, int y, int button);

        // department buttons
        RoundButton deptGD_Button;
        RoundButton deptAD_Button;
        RoundButton deptDM_Button;

        void myMouseReleased(ofMouseEventArgs &args);

    protected:
    private:
};

#endif // MAINMENU_H
