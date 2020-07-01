#include <iostream>
#include "Stack.h"

Stack::Stack()
{
    top = NULL;
}

void Stack::addTop(Creature *fighter)
{
    top = new StackNode(fighter, top);
}

Creature* Stack::getTop()
{
    return top->fighter;
}

void Stack::removeTop()
{
    StackNode *temp = top;
    top = top->next;
    delete temp;
}

bool Stack::isEmpty()
{
    return !top;
}

Stack::~Stack()
{
    while (top)
    {
        removeTop();
    }
}
