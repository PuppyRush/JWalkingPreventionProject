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


struct IMAGE{

	char image[1024];

};

template <typename T>
class Queue
{
	private:

	    T queue[MAX];
	    int front;
	    int rear;
	    int count;

	public :
	    static T eventQ;

	public :

	    Queue(){
		    front = 0;
	      rear = 0;
	      count =0;
	    	    }

	    bool Put(T);  //항목을 기억공간에 저장할 함수.
	    bool Get(T &); //저장된 항목을 꺼내기 위한 함수.
	    bool IsEmpty();

};



#endif /* DETECTOR_QUEUE_H_ */
