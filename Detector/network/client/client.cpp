#include "client.h"


void* Client::BeginClient(NTM* ntm){


		pthread_t monitor_th, raspb_th;

		//ntm->BeginSendImage();




		pthread_create(&monitor_th, NULL, &NetworkToMonitor::getBeginForMonitor , ntm);
		//pthread_create(&monitor_th, NULL, &NetworkToMonitor::getBeginSendImage , &str1);

		return ntm;

}
