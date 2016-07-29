/*
 * NetworkToRapsb.h
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_NETWORKTORASPB_H_
#define DETECTOR_NETWORK_CLIENT_NETWORKTORASPB_H_

#include "client.h"

class NetworkToRaspb {

	public:

		SocketInfo msock;

	public:
		NetworkToRaspb();
		virtual ~NetworkToRaspb();

		void *BeginForRaspb(void);
		static void* getBeginForRaspb(void *con){ return ((NetworkToRaspb *)con)->BeginForRaspb();}
		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);

};

#endif /* DETECTOR_NETWORK_CLIENT_NETWORKTORASPB_H_ */
