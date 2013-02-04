#include "viewmanager.h"

ViewManager::ViewManager()
{
    //ctor
    currentView = MAINMENU;
    currentDept = "GD";

    // create first view with event
    static CustomEvent changeViewEvent;
    changeViewEvent.view = currentView;
    viewChanged(changeViewEvent);

    ofAddListener(CustomEvent::deptChange, this, &ViewManager::deptChanged);
    ofAddListener(CustomEvent::viewChange, this, &ViewManager::viewChanged);
}

void ViewManager::deptChanged(CustomEvent &e)
{
    currentDept = e.dept;
}

void ViewManager::viewChanged(CustomEvent &e)
{
    deleteOldView(currentView);
    currentView = e.view;

    switch (currentView)
    {
    case MAINMENU :
        m_view = new MainMenu();
        break;
    case OVERVIEW :
        o_view = new Overview(currentDept);
        break;
    case WORLDVIEW :
        w_view = new WorldView();
        cout << "set cursor" << endl;
        w_view->setCursor(cursor);
        cout << "set cursor end" << endl;
        break;
    }
}

void ViewManager::deleteOldView(viewsEnum oldView)
{
    switch (oldView)
    {
    case MAINMENU :
        delete m_view;
        break;
    case OVERVIEW :
        delete o_view;
        break;
    case WORLDVIEW :
        delete w_view;
        break;
    }
}

void ViewManager::setCursor(HandCursor &_cursor)
{
    cursor = &_cursor;
}

