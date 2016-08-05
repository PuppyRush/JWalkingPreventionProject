/*
 * mian.cpp

 *
 *  Created on: 2016. 7. 18.
 *      Author: cmk
 */
#include "detector/Detector.h"
#include "network/client/NetworkToMonitor.h"
#include "network/server/server.h"
#include "Main.h"



int main(int argc, char** argv){

	//system("sudo gcc /home/pi/workspace/JW/ControlDeivce.c -l wriringPi -o control");

	char **texts =  readFile();


/////클라이언트 접속하기

	pthread_t client_th;
	NetworkToMonitor *ntm = new NTM();

	ntm->myNumber = atoi(texts[0]);
	memcpy(ntm->monitorIp, texts[1], strlen(texts[1]));
	ntm->tcpPort = atoi(texts[2]);
	ntm->udpPort = atoi(texts[3]);
	int udpSock = ntm->udpSock;

	//pthread_create(&client_th, NULL, &NetworkToMonitor::getBeginForMonitor , ntm);


/////주변 라즈베리 탐색


	pthread_t server_th;
	Server server;
	server.myServerPort = atoi(texts[4]);
	server.myNumber = atoi(texts[0]);
	THREAD_SERVER_BEGIN_PARAMETER th_str;
	th_str.context = (void *)&server;
	sleep(1);

	//pthread_create(&server_th, NULL, &Server::getBeginServer , &th_str);





////////////
	pthread_t detctor_th, timer_th;
	Detector dect;

	dect.thresh = atoi(texts[7]);
	dect.scale_step = atof(texts[6]);
	dect.frame_jump = atoi(texts[5]);
	dect.DETECT_DISTANCE_STD = atoi(texts[8]);
	THREAD_DETECTOR_BEGIN_PARAMETER th_str_detector;
	th_str_detector.context = (void *)&dect;
	th_str_detector.ntm = ntm;
	th_str_detector.udpSock = udpSock;
	//th_str_detector.frame_step = frame_step;
	sleep(1);
	pthread_create(&detctor_th, NULL, &Detector::getBeginDectect , &th_str_detector);
	//pthread_create(&timer_th, NULL, &Detector::getBeginForTimer ,  &dect);
	//pthread_join(server_th, NULL);
	pthread_join(detctor_th, NULL);

//////////
}

char** readFile(){

	char num[2];



	ifstream myfile;
	myfile.open ("config.txt");

	myfile.getline(num,2);

	int varnum = (int)(num[0]-'0');
	char** texts = (char**)malloc(sizeof(char*)*varnum);
	for(int i=0 ; i < varnum ; i++){
		char temp[101] = {0};
		myfile.getline(temp,100);
		texts[i] = (char*)calloc(0,sizeof(char)*strlen(temp)+1);
		memcpy(texts[i], temp, strlen(temp));
		printf("%s\n", texts[i]);
	}

	myfile.close();
	return texts;
}


