#ifndef VIEW_H
#define VIEW_H

#include "viewmanager.h"
#include "cursor.h"

class View
{
public:
    void setup(HandCursor &c, ViewManager &vs);
    virtual void setupButtons();
    void update(bool t);
    bool trackingHand;

    ViewManager* viewmanager;
    HandCursor* cursor;

protected:
    View();
    virtual ~View();
private:
};

#endif // VIEW_H
