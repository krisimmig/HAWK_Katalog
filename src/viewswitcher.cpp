#include "viewmanager.h"

ViewManager::ViewManager()
{
    //ctor
    currentView = MAINMENU;
}

ViewManager::~ViewManager()
{
    //dtor
}

void ViewManager::setView(enum_views view)
{
    currentView = view;
}

enum_views ViewManager::getCurrentView()
{
    return currentView;
}
