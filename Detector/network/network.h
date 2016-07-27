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
#include <arpa/inet.h>
#include "../Queue.h"

#define MAX_BUF 	1024
#define MSG_LEN	128

///////////////////MSG INDEX
enum SERVER_MSG{

	REQ_FIRST_MESSAGE=100,
	SEND_SERVER_MESSAGE

};


enum CLINET_MSG{

	SEND_FIRST_MESSAGE=100,
	SEND_CLIENT_MESSAGE

};


/////////////////MACHINE INFO


#define RASPB	1
#define MONITOR	2


 struct SocketInfo{

	int sockfd;
	int who;
	bool isConnected;
};

struct THREAD_SERVER_PARAMETER{
	 void* context;
	 int sockfd;

 };


//////////////////////MSG HEADER

struct HEADER{

	int msgIdx;
	int who;
	int body_str_size;

};

///////////////////FOR SENDED STRUCT

struct SEND_REQ_INFORMATION{
	HEADER hd;
};

 struct SEND_MESSAGE{
 	HEADER hd;
 	int msgLen;
 	char msg[MSG_LEN];
 };

struct SEND_JWALKING_EVENT_SIGNAL{
	 HEADER hd;
	 EVENT_SIGNAL event;

 };
 ////////////////////FOR RECEIVED STRUCT

 struct ON_MESSAGE{

 	int msgLen;
 	char msg[MSG_LEN];

 };



 char * getIpAddress();


#endif /* DETECTOR_NETWORK_H_ */
