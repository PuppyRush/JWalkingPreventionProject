/*
 * client.h
 *
 *  Created on: 2016. 7. 25.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_CLIENT_H_
#define DETECTOR_NETWORK_CLIENT_CLIENT_H_

#include "NetworkToMonitor.h"
#include <pthread.h>



class Client {

	public:
		void* BeginClient(NTM* );
		 int myNumber;

};



#endif /* DETECTOR_NETWORK_CLIENT_CLIENT_H_ */

