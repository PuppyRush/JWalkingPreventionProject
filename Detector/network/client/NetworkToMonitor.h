/*
 * NetworkToMonitor.h
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_
#define DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_

#include "../network.h"

class NetworkToMonitor {


	public:

		SocketInfo rsock;

	public:

		void *BeginForMonitor(Queue<EVENT_SIGNAL> *q);
		static void* getBeginForMonitor(void* th){
			THREAD_CLIENT_BEGIN_PARAMETER *str = (THREAD_CLIENT_BEGIN_PARAMETER *)th;
			return ( (NetworkToMonitor *)str->context)->BeginForMonitor( str->q );
		}

		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);

};


#endif /* DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_ */
