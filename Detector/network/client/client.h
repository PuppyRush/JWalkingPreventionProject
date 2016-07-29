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
#include "../../network/network.h"

#define RASPB_PORT	9191
#define MONITOR_PORT 9090 /* the port client will be connecting to */
#define MAXDATASIZE 1024 /* max number of bytes we can get at once */



class Client{


	private:


	public:

		void BeginClient();


};



#endif /* DETECTOR_NETWORK_CLIENT_CLIENT_H_ */
