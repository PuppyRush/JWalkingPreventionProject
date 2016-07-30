#include "client.h"


void* Client::BeginClient(){


		pthread_t monitor_th, raspb_th;

		NTR *ntr = new NTR();
		NTM *ntm = new NTM();

		THREAD_NETWORK_BEGIN_PARAMETER str1;
		str1.context = ntm;


		THREAD_NETWORK_BEGIN_PARAMETER str2;
		str1.context = ntr;

		pthread_create(&monitor_th, NULL, &NetworkToMonitor::getBeginForMonitor , &str1);
		//pthread_create(&raspb_th, NULL, &NetworkToRaspb::getBeginForRaspb , &str2);

		return ntm;

}
