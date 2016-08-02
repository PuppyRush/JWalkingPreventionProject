/*
 * network.h
 *
 *  Created on: 2016. 7. 25.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_H_
#define DETECTOR_NETWORK_H_

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
//#include "../Queue.h"

#define MAX_BUF 	1024
#define MSG_LEN	128
#define RASPB_PORT	9191
#define MONITOR_PORT 9090
#define MONITOR_UDP_PORT 9091
#define MYPORT 9191
#define BACKLOG 10     	/* how many pending connections queue will hold */




///////////////////MSG INDEX

enum RASPB_MSG{

	SEND_FIRST_MESSAGE_TO_MONITOR=500,
	SEND_IMAGE,
	SEND_SIGNAL,
	SEND_MESSAGE_TO_MONITOR

};


enum MONITOR_MSG
{
	SEND_SIGNAL_MONITOR = 500
};

/////////////////MACHINE INFO


#define RASPB			1
#define MONITOR		2

/////////////////EVENT KIND
enum EVENT_KIND{

	OCCUR_JWALKER	= 100

};

 struct SocketInfo{

	int sockfd;
	int who;
	bool isConnected;

};

 struct THREAD_NETWORK_BEGIN_PARAMETER{

 	void* context;

 };


struct EVENT_SIGNAL{

 	int signal;
 	int road_num;

 };


struct IMAGE{

	int imageArraySize;
 	unsigned char image[333000];

 };



//////////////////////MSG HEADER

struct HEADER{

	int msgIdx;
	int who;
	int body_str_size;

};

///////////////////FOR BE SENDED STRUCT

struct SEND_REQ_INFORMATION{
	HEADER hd;
};

struct SEND_INFO_TO_MONITOR{
	HEADER hd;
	int raspbnum;
};

 struct SEND_MESSAGE{
 	HEADER hd;
 	int from;
 	int to;
 	int msgLen;
 	char msg[MSG_LEN];
 };

struct SEND_JWALKING_EVENT_SIGNAL{
	 HEADER hd;
	 EVENT_SIGNAL event;
	 int from;
	 int to;

 };

struct SEND_JWALKING_IMAGE{
	 HEADER hd;

	 IMAGE event;
 };
 ////////////////////FOR BE RECEIVED STRUCT

 struct ON_MESSAGE{
	int from;
	int to;
 	int msgLen;
 	char msg[MSG_LEN];

 };

 struct ON_JWALKING_EVENT_SIGNAL{
	 EVENT_SIGNAL event;
 	 int from;
 	 int to;

  };

/* class Network{

 	 private:

	 	static int GetMyRaspNumber();
	 	static char* getIpAddress();

 	 public:

	 	static void SetMyNumber(){
	 		myNumber = GetMyRaspNumber();
	 	}

	 	 static int getNumber(){
	 		 return myNumber;
	 	 }

 };*/


#endif /* DETECTOR_NETWORK_H_ */
