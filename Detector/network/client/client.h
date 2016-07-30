/*
 * client.h
 *
 *  Created on: 2016. 7. 25.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_CLIENT_H_
#define DETECTOR_NETWORK_CLIENT_CLIENT_H_

#include "NetworkToRaspb.h"
#include "NetworkToMonitor.h"




class Client {

	public:
		void* BeginClient();
		 int myNumber;

};



#endif /* DETECTOR_NETWORK_CLIENT_CLIENT_H_ */

