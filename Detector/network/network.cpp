/*
 * network.cpp

 *
 *  Created on: 2016. 7. 26.
 *      Author: cmk
 */

#include "network.h"
/*

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int Network::GetMyRaspNumber(){

	pthread_mutex_lock(&mutex);

	string ip(getIpAddress());
	int idx = ip.find_last_of(".");
	char num[1] = {0};
	num[0]=ip[idx+1];
	return atoi(num);

	pthread_mutex_unlock(&mutex);
}

char* Network::getIpAddress(){

	struct ifaddrs * ifAddrStruct=NULL;
	struct ifaddrs * ifa=NULL;
	void * tmpAddrPtr=NULL;
	char *addressBuffer;
	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
		    continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
		    // is a valid IP4 Address
		    tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
		    addressBuffer = new char[INET_ADDRSTRLEN];
		    inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
		    return addressBuffer;
		  //  printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);

		} else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
		    // is a valid IP6 Address
		    tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
		    addressBuffer = new char[INET6_ADDRSTRLEN];
		    inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
		    //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
		}
	    }
	if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);

	return addressBuffer;

}


*/
