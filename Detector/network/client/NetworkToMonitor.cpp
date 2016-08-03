/*
 * NetworkToMonitor.cpp
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#include "NetworkToMonitor.h"




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

	char buf[1024];

	SEND_INFO_TO_MONITOR str;
	HEADER h;
	h.msgIdx = SEND_FIRST_MESSAGE_TO_MONITOR;
	h.who = RASPB;
	h.body_str_size = sizeof(str) - sizeof(HEADER);
	str.raspbnum = myNumber;
	str.hd = h;
	memcpy(buf, &str, sizeof(str));

	if(send(sockfd, (void *)&str, sizeof(str), 0) <=0 ){
		perror("fail send message");
		return false;
	}
	else
		return true;

}

bool NTM::SendImage(IMAGE image){

	HEADER hd;
	hd.who = RASPB;
	hd.msgIdx =SEND_IMAGE;

	SEND_JWALKING_IMAGE str;
	str.event = image;
	str.hd = hd;

	hd.body_str_size = sizeof(str) - sizeof(hd);



	if(sendto(udpSock, (void *)&str, sizeof(str)/100, 0, (struct sockaddr *)&udpaddr, sizeof(udpaddr)) <=0 ){
		perror("fail send message");
		return false;
	}
	else
		return true;

	/*int sendlen = sendto(udpSock,(char *)&image,sizeof(image),0, (struct sockaddr*)&udpaddr,sizeof(udpaddr));
	printf("%d", sendlen);*/
	return true;
}

bool NetworkToMonitor::SendEventSignal(){

	SEND_JWALKING_EVENT_SIGNAL str;
	str.to = 1;
	str.from = 4;

	EVENT_SIGNAL event;
	event.road_num = 1;
	event.signal = OCCUR_JWALKER;

	HEADER hd;
	hd.who = RASPB;
	hd.msgIdx = SEND_SIGNAL;

	str.hd = hd;
	str.event = event;
	str.hd.body_str_size = sizeof(str)-sizeof(hd)-sizeof(event);

	if(send(monitorSock, (void *)&str, sizeof(str), 0) <=0 ){
		perror("fail send message");
		return false;
	}
	else
		return true;


	return true;
}
