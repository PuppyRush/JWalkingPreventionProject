/*
 * NetworkToMonitor.cpp
 *
 *  Created on: 2016. 7. 29.
 *      Author: cmk
 */

#include "NetworkToRaspb.h"


void *NetworkToRaspb::BeginForRaspb(Queue<EVENT_SIGNAL> *q){

	int sockfd, numbytes;
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */


	he=gethostbyname("localhost");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(RASPB_PORT);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
	    perror("fail connection");
	    //exit(1);
        }
	else{
		SendMessage(sockfd, "Hi Server, I'm a RASPB.\n");
		printf("succ_connection_of_raspb.\n");
	}




	while (1) {


		char buf[sizeof(HEADER)]={0};

		if ((numbytes=recv(sockfd, buf, sizeof(HEADER), 0)) == -1) {
			perror("nothing recv.");
			//exit(1);
		}
		else{
			TranslateMsg(sockfd, buf);
		}
	}

	close(sockfd);

	return 0;
}

bool NetworkToRaspb::TranslateMsg(int sockfd, char* buf){

	HEADER h;
	memcpy(&h, buf, sizeof(HEADER));

	switch(h.msgIdx){

		case REQ_FIRST_MESSAGE:{



			printf("recv REQ_FIRST_MESSAGE OF CLIENT.\n");

			if(h.who == RASPB){
				msock.sockfd = sockfd;
				msock.isConnected = true;
				msock.who = RASPB;
			}

			else{
				fprintf(stderr,"I DONT KNOW WHO IS HE (%d).\n", h.who);
			}

			HEADER h_;
			h_.msgIdx = SEND_FIRST_MESSAGE;
			h_.who = RASPB;
			h.body_str_size = sizeof(SEND_REQ_INFORMATION);
			SEND_REQ_INFORMATION str;
			str.hd = h_;

			if(send(sockfd , (char*)&str, sizeof(SEND_REQ_INFORMATION), 0) >0){
				return true;
			}
			else
				return false;

			break;
		}
		case SEND_SERVER_MESSAGE:{

			ON_MESSAGE str;

			memcpy(&str, &buf[sizeof(HEADER)] , h.body_str_size);
			char *msg = new char[str.msgLen];
			memcpy(&msg, &str.msg, str.msgLen);
			printf("%s\n", msg);
			break;
		}


	}


	return true;
}


bool NetworkToRaspb::SendMessage(int sockfd, char* msg){

	int send_size;
	HEADER h;
	h.msgIdx = SEND_CLIENT_MESSAGE;
	h.who = RASPB;

	int strLen = strlen(msg);

	SEND_MESSAGE str;
	memset(&str,0,sizeof(SEND_MESSAGE));
	memcpy(str.msg, msg, strLen );
	str.msgLen = strLen;

	str.hd = h;

	h.body_str_size = sizeof(str) - sizeof(HEADER);

	if(send_size =send(sockfd, (char*)&str, sizeof(str), 0) <=0 ){
		perror("fail send message");
		return false;
	}
	else
		return true;


}
