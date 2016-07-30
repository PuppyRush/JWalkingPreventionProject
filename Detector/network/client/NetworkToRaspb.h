/*
 * NetworkToRapsb.h
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_NETWORKTORASPB_H_
#define DETECTOR_NETWORK_CLIENT_NETWORKTORASPB_H_

#include "../network.h"

class NetworkToRaspb {


	public:

		SocketInfo msock;

	public:

		void *BeginForRaspb();
		static void* getBeginForRaspb(void* th){
			THREAD_NETWORK_BEGIN_PARAMETER *str = (THREAD_NETWORK_BEGIN_PARAMETER *)th;
			return ( (NetworkToRaspb *)str->context)->BeginForRaspb( );
		}
		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);

};

typedef NetworkToRaspb NTR;

#endif /* DETECTOR_NETWORK_CLIENT_NETWORKTORASPB_H_ */
