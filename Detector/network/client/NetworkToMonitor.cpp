/*
 * NetworkToMonitor.cpp
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#include "NetworkToMonitor.h"



void *NetworkToMonitor::BeginForMonitor(){



	int sockfd, numbytes;
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	he=gethostbyname("192.168.137.1");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(9090);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */
	int err;

	if( (err = connect( sockfd, (struct sockaddr*)&their_addr, sizeof( their_addr) )) == -1 )
	{
		printf( "\n모니터에 접속실패 %d",err);
		exit( 1);
	}
	else{
		SendMessage(sockfd, "Hi Server, I'm a RASPB.\n");
		printf("succ_connection_to_monitor.\n");
	}


	while(1){
		char buf[MAX_BUF];

		if ((numbytes=recv(sockfd, buf, sizeof(HEADER), 0)) == -1) {
			perror("nothing recv.");
			//exit(1);
		}
		else{
			TranslateMsg(sockfd, buf);
		}


		if(!imageQ->isEmpty())
			SendImage(sockfd, imageQ->deQueue());

	}

}

bool NetworkToMonitor::TranslateMsg(int sockfd, char* buf){

	HEADER h;
	memcpy(&h, buf, sizeof(HEADER));

	switch(h.msgIdx){

/*		case SEND_FIRST_MESSAGE_TO_MONITOR:{

			printf("recv REQ_FIRST_MESSAGE OF CLIENT.\n");

			if(h.who != MONITOR){
				msock.sockfd = sockfd;
				msock.isConnected = true;
				msock.who = Network::getNumber();
			}

			else{
				fprintf(stderr,"I DONT KNOW WHO IS HE (%d).\n", h.who);
			}

			HEADER h_;
			h_.msgIdx = SEND_FIRST_MESSAGE;
			h_.who = Network::getNumber();
			h.body_str_size = sizeof(SEND_REQ_INFORMATION);
			SEND_REQ_INFORMATION str;
			str.hd = h_;

			if(send(sockfd , (char*)&str, sizeof(SEND_REQ_INFORMATION), 0) >0){
				return true;
			}
			else
				return false;

			break;
		}*/
		/*case SEND_SERVER_MESSAGE:{

			ON_MESSAGE str;

			memcpy(&str, &buf[sizeof(HEADER)] , h.body_str_size);
			char *msg = new char[str.msgLen];
			memcpy(&msg, &str.msg, str.msgLen);
			printf("%s\n", msg);
			break;
		}
*/

	}

	return true;
}


bool NetworkToMonitor::SendMessage(int sockfd, char* msg){

//	int send_size;
//	HEADER h;
//	h.msgIdx = SEND_CLIENT_MESSAGE;
//	h.who = Network::getNumber();
//
//	int strLen = strlen(msg);
//
//	SEND_MESSAGE str;
//	memset(&str,0,sizeof(SEND_MESSAGE));
//	memcpy(str.msg, msg, strLen );
//	str.msgLen = strLen;
//
//	str.hd = h;
//
//	h.body_str_size = sizeof(str) - sizeof(HEADER);
//
//	if(send_size =send(sockfd, (char*)&str, sizeof(str), 0) <=0 ){
//		perror("fail send message");
//		return false;
//	}
//	else
//		return true;


}

bool NTM::SendFirstMessage(int sockfd){

	SEND_INFO_TO_MONITOR str;

	int send_size;
	HEADER h;
	h.msgIdx = SEND_FIRST_MESSAGE_TO_MONITOR;
	h.who = myNumber;
	h.body_str_size = sizeof(int);
	str.raspbnum = myNumber;

	h.body_str_size = sizeof(str) - sizeof(HEADER);

	if(send_size =send(sockfd, (char*)&str, sizeof(str), 0) <=0 ){
		perror("fail send message");
		return false;
	}
	else
		return true;

}

bool NetworkToMonitor::SendImage(int sockfd, IMAGE image){



	return true;
}

bool NetworkToMonitor::SendEventSignal(int sockfd, EVENT_SIGNAL e){



	return true;
}
