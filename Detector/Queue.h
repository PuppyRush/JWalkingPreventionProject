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

#define MAX 50

struct EVENT_SIGNAL{

	int signal;
	time_t occured_time;

};


class Queue
{
	private:

	    EVENT_SIGNAL queue[MAX];
	    int front;
	    int rear;
	    int count;

	public :
	    static Queue eventQ;

	public :

	    Queue();
	    ~Queue();

	    bool Put(EVENT_SIGNAL);  //항목을 기억공간에 저장할 함수.
	    bool Get(EVENT_SIGNAL &); //저장된 항목을 꺼내기 위한 함수.
	    bool IsEmpty();

};



#endif /* DETECTOR_QUEUE_H_ */
