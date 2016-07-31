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

		SocketInfo rsock;
		SocketInfo msock;
		int udpSock, monitorSock;
		sockaddr_in udpaddr;
		bool isLockImageSend;

	public:

		NetworkToMonitor(const NetworkToMonitor &T){


			rsock = T.rsock;
			msock = T.msock;

			isLockImageSend = T.isLockImageSend;

		}

		NetworkToMonitor(){



		}

		void *BeginForMonitor();
		static void* getBeginForMonitor(void* th){
			THREAD_NETWORK_BEGIN_PARAMETER *str = (THREAD_NETWORK_BEGIN_PARAMETER *)th;
			return ( (NetworkToMonitor *)str->context)->BeginForMonitor();
		}
		void BeginSendImage();


		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);
		bool SendImage(IMAGE image);
		bool SendEventSignal(int sockfd, EVENT_SIGNAL);
		bool SendFirstMessage(int sockfd);

};

typedef NetworkToMonitor NTM;

#endif /* DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_ */
