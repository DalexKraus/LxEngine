#ifndef __LXINPUTMANAGER_H
#define __LXINPUTMANAGER_H

#include "lxwindow.h"

struct lxInputQueueNode
{
    void (*handle_callback)(lxWindow window);
    struct lxInputQueueNode* next;
};
typedef struct lxInputQueueNode* lxInputQueueHandlerNode;

struct lxInputMgrData
{
    lxInputQueueHandlerNode handlerQueueRoot;
};
typedef struct lxInputMgrData* lxInputManager;

// Functions
lxInputManager  lxInputManagerCreate();
void            lxInputManagerDestroy(lxInputManager mgr);
void            lxInputManagerAddHandler(lxInputManager mgr, void (*handle_callback)(lxWindow window));
void            lxInputManagerDelHandler(lxInputManager mgr, void (*handle_callback)(lxWindow window));

#endif