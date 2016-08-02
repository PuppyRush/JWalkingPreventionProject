///////////////////////////////////////////////////////////////////////////
// OpenCV pedestrian detection examples.
// Written  by darkpgmr (http://darkpgmr.tistory.com), 2013


#include "Detector.h"





#define long long int int64

using namespace std;


bool Detector::QueryPerformanceFrequency(int64_t *frequency)
{
    /* Sanity check. */
    assert(frequency != NULL);

    /* gettimeofday reports to microsecond accuracy. */
    *frequency = usec_per_sec;

    return true;
}

bool Detector::QueryPerformanceCounter(int64_t *performance_count)
{
    struct timeval time;

    /* Sanity check. */
    assert(performance_count != NULL);

    /* Grab the current time. */
    gettimeofday(&time, NULL);
    *performance_count = time.tv_usec + /* Microseconds. */
                         time.tv_sec * usec_per_sec; /* Seconds. */

    return true;
}

void *Detector::BeginDectect( int frame_step, NetworkToMonitor *ntm, int u)
{



	this->udpSock = u;
	this->ntm = ntm;

	this->frame_step = frame_step;
	VideoCapture *image = NULL;

	if(isActivitedCam)
		image = new VideoCapture("sample1.avi");
	else
		image = new VideoCapture(filePath);

	if (!image->isOpened())
	{
		cout << "can't open video file" << endl;
		return 0;
	}

	image->set(CV_CAP_PROP_FRAME_WIDTH, 640);
	image->set(CV_CAP_PROP_FRAME_WIDTH, 480);

	detect_haarcascades(image);

	if(image) delete image;

	destroyAllWindows();

}

bool Detector::detect_haarcascades(VideoCapture *vc)
{
	LANE *lane = NULL;
	DetectLine dl;

	// detector (14x28 template)
	string cascadeName = "xml/haarcascade_pedestrian.xml";
	CascadeClassifier detector;
	if( !detector.load( cascadeName ) )
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return false;
	}

	// parameters
/*
	int gr_thr = 6;
	double scale_step = 1.04;
	Size min_obj_sz(40,100);
	Size max_obj_sz(90,180);
*/

	int gr_thr = 40;
	double scale_step = 1.1;
	Size min_obj_sz(40,100);
	Size max_obj_sz(90,180);


	// run
	Mat frame;
	int64 freq,start,finish;
	QueryPerformanceFrequency((int64_t*)&freq);


	while(1)
	{

		for(int i=0 ; i < 5 ; i++)
			*vc >> frame;

		// input image
		*vc >> frame;
		if(frame.empty()) break;

		ntm->SendImage(MatToImageArray(&frame) );

		lane = dl.beginDetectLine(&frame);

		Mat grayed_frame;
		cvtColor(frame,grayed_frame,CV_BGR2GRAY);
		equalizeHist(grayed_frame, grayed_frame);

		QueryPerformanceCounter((int64_t*)&start);

		// detect
		vector<Rect> found;
		detector.detectMultiScale(grayed_frame, found, scale_step, gr_thr, 0, min_obj_sz, max_obj_sz);

		// processing time (fps)
		//removeNotHumanObject(&found);

		if(!isAlarming && DetectOnlyHuman(&found, lane)){
			ntm->SendEventSignal();
			DeviceController::DoDevice();
		}
		// draw results (bounding boxes)
		for(int i=0; i<(int)found.size(); i++)
			rectangle(frame, found[i], Scalar(0,255,0), 2);



		QueryPerformanceCounter((int64_t*)&finish);
		int counter = (int)found.size();
		double fps = freq / double(finish - start + 1);
		char fps_str[20];
		sprintf(fps_str, "FPS: %.1lf / counter:%d", fps,counter);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0,255,0), 2);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0,255,0), 2);


		// display
		imshow(windowName, *lane->frame);
		char ch = waitKey(10);
		if( ch == 27 ) break;				// ESC Key
		else if(ch == 32 )					// SPACE Key
		{
			while((ch = waitKey(10)) != 32 && ch != 27);
			if(ch == 27) break;
		}
	}

	return true;
}


bool Detector::DetectOnlyHuman(vector<Rect> *found, LANE *lane){

	int std_x = (lane->begin_x+lane->end_x)/2;
	if(isDetecting == false)
		for(vector<Rect>::iterator it = found->begin() ; it != found->end() ; it++){
			if(std_x+DETECT_DISTANCE_STD >= it->x)
				isDetecting = true;

		}
	else{
		for(vector<Rect>::iterator it = found->begin() ; it != found->end() ; it++){
			if(std_x-DETECT_DISTANCE_STD >= it->x){
				isAlarming = true;
				isDetecting = false;
				return true;
			}
		}
	}

	return false;
}

IMAGE Detector::MatToImageArray(const Mat* frame){

	IMAGE img;
	Mat dst;
	cv::resize(*frame,dst, Size(320,240), 0,0,CV_INTER_NN);
	int totalSize = dst.total() * dst.elemSize();

	img.imageArraySize = totalSize;

	memcpy(img.image, dst.data, totalSize);

	return img;

}

bool Detector::RemoveNotHumanObject(vector<Rect> *found){

	if(notObject.size()==0)
	{
		for(vector<Rect>::iterator found_iter = found->begin(); found_iter != found->end(); found_iter++ ){
			testifyObject str;
			str.count = 0;
			str.isChange = true;
			str.p.x =  found_iter->x;
			str.p.y =  found_iter->y;
			notObject.push_back(str);
		}
	}


	int check=0;
	for(vector<Rect>::iterator found_iter = found->begin(); found_iter != found->end(); found_iter++ ){
		for(vector<testifyObject>::iterator not_iter = notObject.begin(); not_iter != notObject.end(); ){

			Point comp_p;
			comp_p.x = (*found_iter).x;
			comp_p.y = (*found_iter).y;
			Point dest_p = (*not_iter).p;
			//기존에 있는데 겹치고 횟수 모자람
			if((*not_iter).count <= NOTOBJECT_COUNT_STD &&
				(dest_p.x-NOTOBJECT_RECT_STD <= comp_p.x && dest_p.x+NOTOBJECT_RECT_STD >= comp_p.x  )&&
				(dest_p.y-NOTOBJECT_RECT_STD <= comp_p.y && dest_p.y+NOTOBJECT_RECT_STD >= comp_p.y  )){
				(*not_iter).count++;
				(*not_iter).isChange = true;
				++not_iter;
			}
			//기존에 있는데 겹치고 횟수 초과
			else if((*not_iter).count >= NOTOBJECT_COUNT_STD &&
				(dest_p.x-NOTOBJECT_RECT_STD <= comp_p.x && dest_p.x+NOTOBJECT_RECT_STD >=  comp_p.x )&&
				(dest_p.y-NOTOBJECT_RECT_STD <= comp_p.y && dest_p.y+NOTOBJECT_RECT_STD >=  comp_p.y )){
				found_iter = found->erase(found_iter);
				(*not_iter).isChange = true;
				++not_iter;
			}
			//기존에 있는 객체와 맞는게 없다면 새로추가
			else{
				not_iter->isChange = false;
				check++;
				++not_iter;
			}
		}
		if(check == notObject.size()){
			testifyObject str;
			str.maybeHumanCount = 0;
			str.count = 0;
			str.isChange = true;
			str.p.x = found_iter->x;
			str.p.y = found_iter->y;
			notObject.push_back(str);

		}

		check=0;

	}

	for(vector<testifyObject>::iterator not_iter = notObject.begin(); not_iter != notObject.end(); ){
		if(not_iter->isChange == false){
			if(not_iter->maybeHumanCount >= NOTOBJECT_HUMAN_COUNT_STD)
				not_iter =  notObject.erase(not_iter);
			else{
				not_iter->maybeHumanCount++;
				not_iter++;
			}
		}
		else
			not_iter++;

	}

}

void *Detector::AlramTimer(){

	while(true){
		if(isAlarming){
			sleep(5000);
			isAlarming = false;

		}
	}
}
