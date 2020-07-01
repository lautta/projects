#ifndef STACK_H
#define STACK_H

#include "Creature.h"

class Stack
{
private:
    struct StackNode
    {
        Creature *fighter;
        StackNode *next;
        StackNode(Creature *aFighter, StackNode *aNext = NULL) : fighter(aFighter), next(aNext) {}
    };
    StackNode *top;

public:
    Stack();
    void addTop(Creature*);
    Creature* getTop();
    void removeTop();
    bool isEmpty();
    ~Stack();
};


#endif //STACK_H
