#include "client.h"


#define PORT 3456    /* the port client will be connecting to */
#define MAXDATASIZE 1024 /* max number of bytes we can get at once */

void *Client::BeginClient(void){



	int sockfd, numbytes;
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */


	he=gethostbyname("localhost");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(PORT);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
	    perror("fail connection");
	    //exit(1);
        }
	else{
		printf("succ_connection_of_raspb.\n");
	}
	while (1) {

//		if(initConn)

		/*if (send(sockfd, "Hello, world!\n", 14, 0) == -1){
	      perror("send");
	      exit (1);
		}
		printf("After the send function \n");
*/

		char buf[sizeof(HEADER)]={0};

		if ((numbytes=recv(sockfd, buf, sizeof(HEADER), 0)) == -1) {
			perror("nothing recv.");
			//exit(1);
		}
		else{
			TranslateMsg(sockfd, buf);
		}

		buf[numbytes] = '\0';

		printf("Received in pid=%d, text=: %s. \n",getpid(), buf);
			sleep(1);

	}

	close(sockfd);

	return 0;
}

bool Client::TranslateMsg(int sockfd, char* buf){

	HEADER h;
	memcpy(&h, buf, sizeof(HEADER));

	switch(h.msgIdx){

		case REQ_FIRST_MESSAGE:{

			SendMessage(sockfd, "Hi Server, I'm a RASPB.\n");

			printf("\nrecv REQUEST_INFORMATION_MSG.\n");

			if(h.who == RASPB){
				rsock.sockfd = sockfd;
				rsock.isConnected = true;
				rsock.who = RASPB;
			}
			else if(h.who == MONITOR){
				msock.sockfd = sockfd;
				msock.isConnected = true;
				msock.who = MONITOR;
			}
			else{
				fprintf(stderr,"\nI DONT KNOW WHO IS HE (%d).\n", h.who);
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


bool Client::SendMessage(int sockfd, char* msg){

	HEADER h;
	h.msgIdx = SEND_CLIENT_MESSAGE;
	h.who = RASPB;

	SEND_MESSAGE str;
	str.hd = h;
	str.msgLen = sizeof(msg);
	str.msg = msg;

	h.body_str_size = sizeof(str);

	if(send(sockfd, (char*)&str, sizeof(str), 0) <=0 ){
		perror("fail send message");
		return false;
	}
	else
		return true;


}

