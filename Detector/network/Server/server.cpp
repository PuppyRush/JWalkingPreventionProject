#include "server.h"


void *Server::BeginServer(void)
    {
   struct 	sockaddr_in 	my_addr;    			/*My address information 			   */
	struct 	sockaddr_in 	their_addr; 			/*Connector's address information 	           */
	struct 	timeval 	tv;				/*The time wait for socket to be changed	   */
	struct 	timezone	tz;				/*The time zone we are using 			   */
	struct 	timeval 	measure_tv_before;		/*The time wait for socket to be changed	   */
	struct 	timezone	measure_tz_before;		/*The time zone we are using 			   */
	struct 	timeval 	measure_tv_after;		/*The time wait for socket to be changed	   */
	struct 	timezone	measure_tz_after;		/*The time zone we are using 			   */
	fd_set 			readfds, writefds, exceptfds; 	/*File descriptors for read, write and exceptions  */
	int 			new_fd;  			/*The new file descriptor returned from accept	   */
	int 			sockfd;  			/*The first file descriptor given for communication*/
	int			j;				/*Index used for fd counter			   */
	socklen_t 			sin_size;			/*Size of struct internet input address		   */
	int			numbytes;			/* Number of bytes received in each packet         */
	int			sock;
	int			sock_to_read;			/* File descriptor ready for reading		   */
	int			max_fd;				/* Number of socket fd that are in use		   */
	char        *buf;       	/*The string to be passed 			   */

	buf = new char[1024];



	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
        }


	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
								      == -1) {
	    perror("bind");
	    exit(1);
        }

	if (listen(sockfd, BACKLOG) == -1) {
	    perror("listen");
	    exit(1);
        }

	if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
		usleep(100);
	}

}


bool Server::TranslateMsg(int sockfd){

	char buf[1024]={0};

	HEADER h;
	memcpy(&h, buf, sizeof(HEADER));

	switch(h.msgIdx){

		case SEND_FIRST_MESSAGE:{

			printf("recv SEND_FIRST_MESSAGE OF SERVER\n");

			if(h.who == RASPB){
				rsock.isConnected = true;
				rsock.sockfd = sockfd;
				rsock.who = RASPB;
			}
			else if(h.who == MONITOR){
				msock.isConnected = true;
				msock.sockfd = sockfd;
				msock.who = MONITOR;
			}


			break;
		}
		case SEND_CLIENT_MESSAGE:{

			ON_MESSAGE str;

			memcpy(&str, &buf[sizeof(HEADER)] , h.body_str_size);
			char *msg = new char[str.msgLen];
			memcpy(&msg, &str.msg, str.msgLen);
			printf("%s\n", msg);
			break;
		}

	}

	usleep(10);

	return true;
}



bool Server::ReuqestWhoisyou(int sockfd){

	SEND_REQ_INFORMATION str;
	str.hd.msgIdx=REQ_FIRST_MESSAGE;
	str.hd.who = RASPB;
	str.hd.body_str_size = 0;

	if(send(sockfd , (char*)&str, sizeof(str), 0) >0){
		printf("send REQ_FIRST_MESSAGE\n");
		return true;
	}
	else
		return false;
	usleep(10);

}

bool Server::SendMessage(int sockfd, char* msg){

	HEADER h;
	h.msgIdx = SEND_SERVER_MESSAGE;
	h.who = RASPB;

	SEND_MESSAGE str;
	str.hd = h;
	str.msgLen = strlen(msg);
	str.msg = msg;

	h.body_str_size = sizeof(str);

	if(send(sockfd, (char*)&str, sizeof(str), 0) <=0 ){
		perror("fail send message");
		return false;
	}
	else
		return true;

	return true;

	usleep(10);
}


