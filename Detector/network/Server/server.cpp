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

	/*Initiate the read, write, except structs */
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);


	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
        }

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
								      == -1) {
	    perror("bind");
	    exit(1);
        }

	if (listen(sockfd, BACKLOG) == -1) {
	    perror("listen");
	    exit(1);
        }

	max_fd = sockfd;
	FD_SET(sockfd, &readfds); /*Add sock_fd to the set of file descriptors to read from */
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	for(j=sockfd;j<=max_fd;j++){
		FD_SET(j,&readfds);
		printf ("Setup fd number %d \n",j);
	}

	while(1) {

	    gettimeofday(&measure_tv_before,&measure_tz_before);
	    tv.tv_sec = 5;
	    if ((sock = select(max_fd+1 , &readfds, &writefds, &exceptfds, &tv)) < 0) {
	       perror("Nothing changed fds");
	       continue;
	    	    }


	    printf("Sock value is (after select numbers) :%d \n",sock);

		if(FD_ISSET(sockfd, &readfds)){

			if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
				usleep(100);
				perror("accept error\n");
				continue;
			}
			else{
				printf("new accept\n");
				SendMessage(sock_to_read, "I'm a RaspberryServer\n");

				//recv(sock_to_read, buf, 1024, 0);
				ReuqestWhoisyou(new_fd);

			}

			max_fd = new_fd;
			printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
			FD_SET(new_fd, &readfds);
			FD_CLR(sockfd, &readfds);
			//printf("Ending the connect function %ld, %ld\n",tv.tv_sec,tv.tv_usec);

		}
		else {
		    /*Check which FD are set */
		    for (j=sockfd;j<=max_fd && sock > 0;j++){	/*The loop starts from the first comm fd */
			    //printf("Checking now j=%d, readfs(j) status =%d \n", j, FD_ISSET(j, &readfds));
			    if (FD_ISSET(j, &readfds) == 1 ) {
					sock_to_read = j;

					if ((numbytes=recv(sock_to_read, buf, 1024, 0)) == -1){
						perror("recv");
						exit(1);
					}
					else
						TranslateMsg(sock_to_read);

					buf[numbytes] = '\0';
					printf("\nrecv: sock=%d, buf=%s \n", sock,buf);

/*
					if (send(sock_to_read , "\n I'm a RaspberryServer", 25, 0) == -1)
						perror("error in sending\n");*/

					FD_CLR(sock_to_read, &readfds);
					sock--; /* We found one fd that was changed form select	*/
					for(j=sockfd;j<=max_fd;j++)
						FD_SET(j,&readfds);
				}
		    }
	    }

	}
	close(new_fd);


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


