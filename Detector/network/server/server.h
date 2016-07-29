/*
 * server.h
 *
 *  Created on: 2016. 7. 25.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_SERVER_SERVER_H_
#define DETECTOR_NETWORK_SERVER_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../../network/network.h"
#include "../../Queue.h"

struct THREAD_SERVER_PARAMETER{
	 void* context;
	 int sockfd;

 };

struct THREAD_SERVER_BEGIN_PARAMETER{

	void* context;
	Queue *q;

};

#define MYPORT 3456    	/* the port users will be connecting to */
#define MAXDATASIZE 100	/*The maximum message passed */
#define BACKLOG 10     	/* how many pending connections queue will hold */



class Server{


	private:
		Queue *eventQ;
		SocketInfo rsock, msock;

	public:

		Server(){
			eventQ = NULL;
		}

		void *BeginServer(Queue *q);
		static void* getBeginServer(void* th){
			THREAD_SERVER_BEGIN_PARAMETER *str = (THREAD_SERVER_BEGIN_PARAMETER *)th;
			return ( (Server *)str->context)->BeginServer(str->q);
		}

		void *Receive(int sockfd);
		static void* getReceive(void* th){
			THREAD_SERVER_PARAMETER *str = (THREAD_SERVER_PARAMETER *)th;
			return ( (Server *)str->context)->Receive( str->sockfd );
		}
		void *CheckQueue(void);
		static void* getCheckQueue(void *con){ return ((Server *)con)->CheckQueue();}

		bool TranslateMsg(int sockfd, char* buf);
		bool ReuqestWhoisyou(int sockfd);
		bool SendMessage(int sockfd, char* msg);




};




#endif /* DETECTOR_NETWORK_SERVER_SERVER_H_ */