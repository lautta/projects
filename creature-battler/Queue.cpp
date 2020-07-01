#include <iostream>
#include "Queue.h"

Queue::Queue()
{
    front = back = NULL;
}

void Queue::addBack(Creature *fighter)
{
    if(!front)
    {
        front = new QueueNode(fighter, NULL);
        back = front;
    }
    else
    {
        back->next = new QueueNode(fighter, NULL);
        back = back->next;
    }
}

Creature* Queue::getFront()
{
    return front->fighter;
}

void Queue::removeFront()
{
    QueueNode *temp = front;
    front = front->next;
    delete temp;
}

bool Queue::isEmpty()
{
    return !front;
}

Queue::~Queue()
{
    while (front)
    {
        removeFront();
    }
}
