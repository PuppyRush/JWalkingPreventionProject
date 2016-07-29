/*
 * NetworkToMonitor.h
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_
#define DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_

#include "client.h"



class NetworkToMonitor {

	public:

		SocketInfo rsock;

	public:
		NetworkToMonitor();
		virtual ~NetworkToMonitor();

		void *BeginForMonitor(void);
		static void* getBeginForMonitor(void *con){ return ((NetworkToMonitor *)con)->BeginForMonitor();}
		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);

};


#endif /* DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_ */
