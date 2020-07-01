#ifndef QUEUE_H
#define QUEUE_H

#include "Creature.h"

class Queue
{
private:
    struct QueueNode
    {
        Creature *fighter;
        QueueNode *next;
        QueueNode(Creature *aFighter, QueueNode *aNext = NULL) : fighter(aFighter), next(aNext) {}
    };
    QueueNode *front;
    QueueNode *back;

public:
    Queue();
    void addBack(Creature*);
    Creature* getFront();
    void removeFront();
    bool isEmpty();
    ~Queue();
};

#endif //QUEUE_H
