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
#include "../network.h"

#endif /* DETECTOR_NETWORK_SERVER_SERVER_H_ */

#define MYPORT 3456    	/* the port users will be connecting to */
#define MAXDATASIZE 100	/*The maximum message passed */
#define BACKLOG 10     	/* how many pending connections queue will hold */


class Server{


	private:

		SocketInfo rsock, msock;

	public:

		void *BeginServer(void);
		static void* getBeginServer(void *con){ return ((Server *)con)->BeginServer();}
		bool TranslateMsg(int sockfd);
		bool ReuqestWhoisyou(int sockfd);
		bool SendMessage(int sockfd, char* msg);




};

