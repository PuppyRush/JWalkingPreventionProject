/*
 * mian.cpp
 *
 *  Created on: 2016. 7. 18.
 *      Author: cmk
 */

#include "Main.h"



int main(int argc, char** argv){

	if(argc < 3 || argc > 4){
		fprintf(stderr, "\n매개변수를 확인하세요\n1.$(모니터 IP)  2.($상대 라즈베리IP) \n"
		"3.($파일을 읽어서 영상처리 테스트할경우 파일 전체경로. default=CAM )  4.($건너띌 프레임 수(default=2))\n");
		return 0;
	}

	char *monip = argv[1];
	char *rasIp = argv[2];
	char *filePath = argv[3];
	int frame_step = 2;
	if(argv[4]!=NULL){
		frame_step = atoi(argv[4]);
	}

///////주변장치 연결확인

	printf("\n주변장치와 연결여부를 확인합니다...\m");

	while(0){

		int conn_state=0;
		char   ch_key=0;

		if( (conn_state = CHECK_DEVICES()) != FULL_CON){

			if(conn_state & 0001){
				fprintf(stderr,"\n캠이 연결되어 있지 않습니다. 캠 사용여부를 다시 확인 후 y를 입력하세요."
						"(\n프로그램 종료를 원하면 n을 입력하세요.");

			}
			else if(conn_state == ( CON_CAM | CON_LED) ){
				fprintf(stderr,"\n스피커가 연결되지 않았습니다.");

			}
			else if(conn_state == (CON_CAM|CON_SPK)){
				fprintf(stderr,"\nLED가 연결되지 않았습니다.");

			}


			while(1){
				ch_key = getchar();

				if(ch_key == 'y'){
					break;
				}

				else if(ch_key == 'n'){
					printf("프로그램을 종료합니다.");
					return 0;
				}
				else
					fprintf(stderr,"\n y나 n만 입력바랍니다.");

			}

			for(int i=0 ; i < 5 ; i++){
				//sleep(1000);
				printf("\n재 연결 시도중입니다..............\n");
			}

		}
		else{
			printf("모든 장치가 연결되어 있습니다.");
			break;
		}
	}

/////모든 클래스에서 사용 할 EventQueue 생성(공유하게됨)
	Queue eventQ;

/////클라이언트 접속하기



/////주변 라즈베리 탐색

	pthread_t server_th, detctor_th;

	Server server;
	THREAD_SERVER_BEGIN_PARAMETER th_str;
	th_str.context = (void *)&server;
	th_str.q = &eventQ;

	pthread_create(&server_th, NULL, &Server::getBeginServer , &th_str);





////////////
	Detector dect;
	THREAD_DETECTOR_BEGIN_PARAMETER th_str_detector;
	th_str_detector.context = (void *)&dect;
	th_str_detector.q = &eventQ;

	pthread_create(&detctor_th, NULL, &Detector::getBeginDectect , &th_str_detector);

	pthread_join(server_th, NULL);
	pthread_join(detctor_th, NULL);

//////////
}
