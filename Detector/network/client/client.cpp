#include "client.h"


void* Client::BeginClient(NTM* ntm){


		pthread_t monitor_th, raspb_th;




		//ntm->BeginSendImage();

		THREAD_NETWORK_BEGIN_PARAMETER str1;
		str1.context = ntm;


		pthread_create(&monitor_th, NULL, &NetworkToMonitor::getBeginForMonitor , &str1);
		//pthread_create(&monitor_th, NULL, &NetworkToMonitor::getBeginSendImage , &str1);

		return ntm;

}
