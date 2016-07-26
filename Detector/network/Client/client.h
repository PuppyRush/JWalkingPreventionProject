/*
 * client.h
 *
 *  Created on: 2016. 7. 25.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_CLIENT_H_
#define DETECTOR_NETWORK_CLIENT_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../network.h"


#endif /* DETECTOR_NETWORK_CLIENT_CLIENT_H_ */

class Client{


	private:

		SocketInfo rsock, msock;


	public:

		void *BeginClient(void);
		static void* getBeginClient(void *con){ return ((Client *)con)->BeginClient();}
		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);

};


