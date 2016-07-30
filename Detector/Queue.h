/*
 * Queue.h
 *
 *  Created on: 2016. 7. 28.
 *      Author: cmk
 */

#ifndef DETECTOR_QUEUE_H_
#define DETECTOR_QUEUE_H_

#include <iostream>
using namespace std;


#define MAX_SIZE 100

using namespace std;

template< class T > class Queue
{
    public:
        Queue();//default constructor
        ~Queue()//destructor
          {delete [] values;}
        bool enQueue( T );
        T deQueue();
        bool isEmpty();
        bool isFull();

    private:
        int size;
        T *values;
        int front;
        int back;

};

template< class T > Queue<T>::Queue():
    size(MAX_SIZE),//ctor
    values(new T[MAX_SIZE]),
    front(0),
    back(0)
      { /*empty*/  }

template< class T > bool Queue<T>::isFull()
{
    if((back + 1) %  size == front )
        return 1;
    else
        return 0;
}

template< class T > bool Queue<T>::enQueue(T x)
{
    bool b = 0;
   if(!Queue<T>::isFull())
   {
       values[back] = x;
       back = (back + 1) % size;
       b = 1;
   }
  return b;
}

template< class T > bool Queue<T>::isEmpty()
{
    if( back  == front )//is empty
        return 1;
    else
    return 0; //is not empty
}

template< class T > T Queue<T>::deQueue()
{
	T val;

	if(!Queue<T>::isEmpty()){
		val = values[front];
		front = ( front + 1 ) % size;
	}
	else
		cerr << "Queue is Empty : ";

	return val;
}

#endif /* DETECTOR_QUEUE_H_ */
