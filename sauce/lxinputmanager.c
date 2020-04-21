#include "lxinputmanager.h"

// Functions
lxInputManager lxInputManagerCreate()
{
    lxInputManager instance = malloc(sizeof(struct lxInputMgrData));
    instance->handlerQueueRoot = 0;
    return instance;
}

void lxInputManagerDestroy(lxInputManager* mgr)
{
    if (mgr != 0 && *mgr != 0)
    {
        free(*mgr);
        mgr = 0;
    }
}

void lxInputManagerAddHandler(lxInputManager mgr, void (*handle_callback)())
{
    if (mgr == 0 || handle_callback == 0)
        return;

    lxInputQueueHandlerNode currentNode = mgr->handlerQueueRoot;
    while (currentNode != 0)
    {
        currentNode = currentNode->next;
    }

    currentNode->next = malloc(sizeof(lxInputQueueHandlerNode));
    currentNode->next->handle_callback = handle_callback;
}

void lxInputManagerDelHandler(lxInputManager mgr, void (*handle_callback)())
{
    if (mgr == 0 || mgr->handlerQueueRoot == 0)
        return;

    lxInputQueueHandlerNode toDelete = 0;
    if (mgr->handlerQueueRoot->handle_callback = handle_callback)
    {
        toDelete = mgr->handlerQueueRoot;
        mgr->handlerQueueRoot = 0;
    }

    lxInputQueueHandlerNode currentNode = mgr->handlerQueueRoot;
    while (currentNode->next != 0)
    {
        if (currentNode->next->handle_callback = handle_callback)
        {
            toDelete = currentNode->next;
            currentNode->next = currentNode->next->next;
            break;
        }
        currentNode = currentNode->next;
    }

    if (toDelete != 0)
    {
        free(toDelete);
    }
}