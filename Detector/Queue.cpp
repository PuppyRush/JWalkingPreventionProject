/*
 * Queue.cpp
 *
 *  Created on: 2016. 7. 28.
 *      Author: cmk
 */

#include "Queue.h"

Queue::Queue()
{
    front = 0;
    rear = 0;
    count =0;
}
Queue::~Queue(){}

bool Queue::Put(EVENT_SIGNAL _d)
{
    if ((rear + 1) % MAX == front)
    {
        return false;
    }

    queue[rear] = _d;
    rear = ++rear % MAX;
    count++;
    return true;
}


bool Queue::Get(EVENT_SIGNAL &_d)
{
    if (front == rear)
    {
        return false;
    }

    _d = queue[front];
    front = ++front % MAX;
    count--;
    return true;

}

bool Queue::IsEmpty(){

	if(count==0)
		return true;
	else
		return false;


}
