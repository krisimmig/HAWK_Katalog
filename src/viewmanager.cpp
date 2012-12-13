#include "viewmanager.h"

ViewManager::ViewManager()
{
    //ctor
    currentView = MAINMENU;
}

void ViewManager::setView(enum_views v)
{
    currentView = v;
}

enum_views ViewManager::getCurrentView()
{
    return currentView;
}
