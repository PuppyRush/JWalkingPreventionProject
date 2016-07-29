/*
 * Queue.cpp
 *
 *  Created on: 2016. 7. 28.
 *      Author: cmk
 */

#include "Queue.h"




template <typename T>
bool Queue<T>::Put(T _d)
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

template <typename T>
bool Queue<T>::Get(T &_d)
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

template <typename T>
bool Queue<T>::IsEmpty(){

	if(count==0)
		return true;
	else
		return false;


}
