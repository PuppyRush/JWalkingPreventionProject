/*
 * NetworkToMonitor.h
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#ifndef DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_
#define DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_

#include <typeinfo>
#include "../network.h"



class NetworkToMonitor {

	public:
		int myNumber;
		int tcpPort, udpPort;
		char monitorIp[30];
		SocketInfo rsock;
		SocketInfo msock;
		int udpSock, monitorSock;
		sockaddr_in udpaddr;
		bool isLockImageSend;

	public:

		NetworkToMonitor(const NetworkToMonitor &T){


			rsock = T.rsock;
			msock = T.msock;

			isLockImageSend = T.isLockImageSend;

		}

		NetworkToMonitor(){



		}

		void *BeginForMonitor(){



			 udpSock=socket(AF_INET, SOCK_DGRAM, 0);

			 bzero(&udpaddr, sizeof(udpaddr));
			 udpaddr.sin_family=AF_INET;
			 udpaddr.sin_port=htons(MONITOR_UDP_PORT);
			 udpaddr.sin_addr.s_addr=inet_addr("192.168.137.1");

			int numbytes;
			struct hostent *he;
			struct sockaddr_in their_addr; /* connector's address information */

			he=gethostbyname("192.168.137.1");

			if ((monitorSock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			    perror("socket");
			    exit(1);
			}

			their_addr.sin_family = AF_INET;      /* host byte order */
			their_addr.sin_port = htons(tcpPort);    /* short, network byte order */
			their_addr.sin_addr = *((struct in_addr *)he->h_addr);
			bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */
			int err;

			if( (err = connect( monitorSock, (struct sockaddr*)&their_addr, sizeof( their_addr) )) == -1 )
			{
				printf( "\n모니터에 접속실패 %d",err);
				exit( 1);
			}
			else{
				SendFirstMessage(monitorSock);
				printf("succ_connection_to_monitor.\n");
			}


			while(1){
				char buf[MAX_BUF];

				if ((numbytes=recv(monitorSock, buf, sizeof(HEADER), 0)) == -1) {
					perror("nothing recv.");
					//exit(1);
				}
				else{
					TranslateMsg(monitorSock, buf);
				}




			}


		}
		static void* getBeginForMonitor(void* th){

			return ( (NetworkToMonitor *)th)->BeginForMonitor();
		}
		void BeginSendImage();


		bool TranslateMsg(int sockfd, char* buf);
		bool SendMessage(int sockfd, char* msg);
		bool SendImage( IMAGE image);
		bool SendEventSignal();
		bool SendFirstMessage(int sockfd);

};

typedef NetworkToMonitor NTM;

#endif /* DETECTOR_NETWORK_CLIENT_NETWORKTOMONITOR_H_ */
