/*
 * network.cpp

 *
 *  Created on: 2016. 7. 26.
 *      Author: cmk
 */

#include "network.h"


char* getIpAddress(){

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