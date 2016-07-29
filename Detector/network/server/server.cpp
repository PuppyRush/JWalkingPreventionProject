#include "../server/server.h"


void *Server::BeginServer(Queue *q)
    {
   struct 	sockaddr_in 	my_addr;    			/*My address information 			   */
	struct 	sockaddr_in 	their_addr; 			/*Connector's address information 	           */
	int 			new_fd;  			/*The new file descriptor returned from accept	   */
	int 			sockfd;  			/*The first file descriptor given for communication*/
	socklen_t 			sin_size;			/*Size of struct internet input address		   */
	char        *buf;       	/*The string to be passed 			   */

	buf = new char[1024];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    perror("socket");
	    exit(1);
        }

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
	    perror("bind");
	    exit(1);
        }
	else
		printf("succ_bind\n");

	if (listen(sockfd, BACKLOG) == -1) {
	    perror("listen");
	    exit(1);
        }
	else
		printf("succ_listen\n");

	while(1){
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) >0){
			printf("succ new accept\n");
			pthread_t sock_th;
			THREAD_SERVER_PARAMETER *par = (THREAD_SERVER_PARAMETER *) malloc(sizeof(THREAD_SERVER_PARAMETER));
			par->context = new Server;
			par->sockfd = new_fd;
			pthread_create(&sock_th, NULL, &Server::getReceive , (void *)par);
			usleep(100);
		}
		else{


		}

		usleep(100);
	}

}

void *Server::Receive(int sockfd){

	int recv_bytes;

	while(true){

		char *buf = new char[MAX_BUF];

		if ((recv_bytes = recv(sockfd, buf, MAX_BUF, 0)) >0){
			printf("suc");
			TranslateMsg(sockfd, buf);
			usleep(50);
		}
		else{
			perror("recv error");
			//exit(1);
		}
	}
}

bool Server::TranslateMsg(int sockfd, char* buf){

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
			char *msg = new char[str.msgLen+1];
			memcpy(msg, &str.msg, str.msgLen);
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

		int send_size;
		HEADER h;
		h.msgIdx = SEND_CLIENT_MESSAGE;
		h.who = RASPB;
		h.body_str_size = sizeof(SEND_MESSAGE) - sizeof(HEADER);

		int strLen = strlen(msg);

		SEND_MESSAGE str;
		memset(&str,0,sizeof(SEND_MESSAGE));
		memcpy(str.msg, msg, strLen );
		str.msgLen = strLen;

		str.hd = h;

		if(send_size =send(sockfd, (char*)&str, sizeof(str), 0) <=0 ){
			perror("fail send message");
			return false;
		}
		else
			return true;

}

