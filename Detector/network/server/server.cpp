#include "../server/server.h"


void *Server::BeginServer(int myNumberPort)
    {


	  char buffer[MAX_BUF];
	    struct sockaddr_in server_addr, client_addr;
	    char temp[20];
	    int server_fd, client_fd;
	    //server_fd, client_fd : 각 소켓 번호
	    int  msg_size;
	    socklen_t len;



	    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	    {// 소켓 생성
	        printf("Server : Can't open stream socket\n");
	        exit(0);
	    }
	    memset(&server_addr, 0x00, sizeof(server_addr));
	    //server_Addr 을 NULL로 초기화

	    server_addr.sin_family = AF_INET;
	    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    server_addr.sin_port = htons(myServerPort);
	    //server_addr 셋팅

	    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
	    {//bind() 호출
	        printf("Server : Can't bind local address.\n");
	        exit(0);
	    	    }

	    if(listen(server_fd, 5) < 0)
	    {//소켓을 수동 대기모드로 설정
	        printf("Server : Can't listening connect.\n");
	        exit(0);
	    }

	    memset(buffer, 0x00, sizeof(buffer));
	    printf("Server : wating connection request.\n");
	    len = sizeof(client_addr);
	    while(1)
		{
			client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
			if(client_fd < 0)
			{
			    printf("Server: accept failed.\n");
			    exit(0);
			}

			pthread_t sock_th;
			THREAD_RECEIVE_PARAMETER *par = (THREAD_RECEIVE_PARAMETER *) malloc(sizeof(THREAD_RECEIVE_PARAMETER));
			par->context = this;
			par->sockfd = client_fd;
			pthread_create(&sock_th, NULL, &Server::getReceive , (void *)par);
			pthread_join(sock_th, NULL);


		}




}

void *Server::Receive(int sockfd){

	int recv_bytes;

	while(true){

		char *buf = new char[MAX_BUF];

		if ((recv_bytes = recv(sockfd, buf, MAX_BUF, 0)) >0){
			printf("succ");
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

	if(h.who == RASPB)
		switch(h.msgIdx){

		/*	case REQ_FIRST_MESSAGE_TO_RASPB:{

				printf("recv SEND_FIRST_MESSAGE OF SERVER\n");

				if(h.who == RASPB){
					rsock.isConnected = true;
					rsock.sockfd = sockfd;
					rsock.who = myNumber;
				}
				else if(h.who == MONITOR){
					msock.isConnected = true;
					msock.sockfd = sockfd;
					msock.who = MONITOR;
				}

				break;
			}
			case SEND_MONITOR_MESSAGE:{

				ON_MESSAGE str;

				memcpy(&str, &buf[sizeof(HEADER)] , h.body_str_size);
				char *msg = new char[str.msgLen+1];
				memcpy(msg, &str.msg, str.msgLen);
				printf("%s\n", msg);
				break;
			}*/

		}
	else if(h.who == MONITOR){
		switch(h.msgIdx){
			case SEND_SIGNAL_MONITOR:{

				EVENT_SIGNAL event;
				int others[2];
				memcpy(&event, &buf[sizeof(HEADER)] , sizeof(event));
				memcpy(others, &buf[sizeof(HEADER) + sizeof(event)] , sizeof(int)*2);
				system("sudo /home/pi/workspace/JW/device/control");
			}
		}

	}

	return true;
}


/*
bool Server::ReuqestWhoisyou(int sockfd){

	SEND_REQ_INFORMATION str;
	str.hd.msgIdx=REQ_FIRST_MESSAGE;
	str.hd.who = myNumber;
	str.hd.body_str_size = 0;

	if(send(sockfd , (char*)&str, sizeof(str), 0) >0){
		printf("send REQ_FIRST_MESSAGE\n");
		return true;
	}
	else
		return false;
	usleep(10);

}*/
/*
bool Server::SendMessage(int sockfd, char* msg){

		int send_size;
		HEADER h;
		h.msgIdx = SEND_CLIENT_MESSAGE;
		h.who = myNumber;
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

}*/

