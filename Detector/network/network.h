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
 	char *msg;
 };
 ////////////////////FOR RECEIVED STRUCT

 struct ON_MESSAGE{
 	int msgLen;
 	char *msg;
 };




#endif /* DETECTOR_NETWORK_H_ */


char * getIpAddress();
