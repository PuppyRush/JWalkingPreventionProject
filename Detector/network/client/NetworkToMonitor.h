/*
 * NetworkToMonitor.h
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_
#define DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_

#include <typeinfo>
#include "../network.h"



class NetworkToMonitor {

	public:
		Queue<EVENT_SIGNAL> *eventQ;
		Queue<IMAGE> *imageQ;
		SocketInfo rsock;
		SocketInfo msock;
		bool isLockImageSend;

	public:

		NetworkToMonitor(const NetworkToMonitor &T){

			eventQ = T.eventQ;
			imageQ = T.imageQ;
			rsock = T.rsock;
			msock = T.msock;

			isLockImageSend = T.isLockImageSend;

		}

		NetworkToMonitor(){

			eventQ = new Queue<EVENT_SIGNAL>();
			imageQ = new Queue<IMAGE>();

		}

		void *BeginForMonitor();
		static void* getBeginForMonitor(void* th){
			THREAD_NETWORK_BEGIN_PARAMETER *str = (THREAD_NETWORK_BEGIN_PARAMETER *)th;
			return ( (NetworkToMonitor *)str->context)->BeginForMonitor();
		}

		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);
		bool SendImage(int sockfd, IMAGE);
		bool SendEventSignal(int sockfd, EVENT_SIGNAL);
		bool SendFirstMessage(int sockfd);

};

typedef NetworkToMonitor NTM;

#endif /* DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_ */
