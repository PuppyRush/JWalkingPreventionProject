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

void *Detector::BeginDectect(NetworkToMonitor *ntm, int u)
{

	pthread_t device_th;
	pthread_create(&device_th, NULL, DoDevice, NULL);


	this->udpSock = u;
	this->ntm = ntm;

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
	double scalestep = scale_step;
	Size min_obj_sz(40,100);
	Size max_obj_sz(90,180);


	// run
	Mat frame;
	int64 freq,start,finish;
	QueryPerformanceFrequency((int64_t*)&freq);


	while(1)
	{

		for(int i=0 ; i < frame_jump ; i++)
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
		detector.detectMultiScale(grayed_frame, found, scalestep, gr_thr, 0, min_obj_sz, max_obj_sz);

		// processing time (fps)
		if(countForLearning <= LEARNING_COUNT)
			LearningNotObject(&found);
		else{
			found = RemoveNotHumanObject(found);
			if(DetectOnlyHuman(&found, lane)){
				ntm->SendEventSignal();
				DoDevice();
			}
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

	if(isAlarming)
		return false;

	int std_x = (lane->begin_x+lane->end_x)/2;
	if(isDetecting == false)
		for(vector<Rect>::iterator it = found->begin() ; it != found->end() ; it++){
			if(std_x+DETECT_DISTANCE_STD >= it->x)
				isDetecting = true;

		}
	else{
		for(vector<Rect>::iterator it = found->begin() ; it != found->end() ; it++){
			if(std_x-DETECT_DISTANCE_STD >= it->x && std_x-DETECT_DISTANCE_STD*2 <= it->x  ){
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

vector<Rect> Detector::RemoveNotHumanObject(vector<Rect> found){

	Point notObj_p;
	Point found_p;

	for(vector<testifyObject>::iterator not_iter = notObject.begin(); not_iter != notObject.end(); not_iter++ ){
		notObj_p = (*not_iter).p;
		for(vector<Rect>::iterator found_iter = found.begin(); found_iter != found.end(); ){
			found_p.x = found_iter->x;
			found_p.y = found_iter->y;

			if((notObj_p.x-NOTOBJECT_RECT_STD <= found_p.x && notObj_p.x+NOTOBJECT_RECT_STD >= found_p.x  )&&
				(notObj_p.y-NOTOBJECT_RECT_STD <= found_p.y && notObj_p.y+NOTOBJECT_RECT_STD >= found_p.y  )){
					found_iter = found.erase(found_iter);
				}
			else
				found_iter++;

		}

	}

	return found;

}

void Detector::LearningNotObject( vector<Rect> *found){

	int check=0;
	Point comp_p;
	Point dest_p;
	Point temp_p;

	if(notObject.size()==0)
	{
		for(vector<Rect>::iterator found_iter = found->begin(); found_iter != found->end(); found_iter++ ){
			testifyObject str;
			str.count = 0;
			str.p.x =  found_iter->x;
			str.p.y =  found_iter->y;
			notObject.push_back(str);
		}
		return;
	}

	for(vector<testifyObject>::iterator not_iter = notObject.begin(); not_iter != notObject.end(); not_iter++){
		dest_p = (*not_iter).p;
		for(vector<Rect>::iterator found_iter = found->begin(); found_iter != found->end(); found_iter++ ){


			comp_p.x = (*found_iter).x;
			comp_p.y = (*found_iter).y;


			if((dest_p.x-NOTOBJECT_RECT_STD <= comp_p.x && dest_p.x+NOTOBJECT_RECT_STD >= comp_p.x  )&&
			(dest_p.y-NOTOBJECT_RECT_STD <= comp_p.y && dest_p.y+NOTOBJECT_RECT_STD >= comp_p.y  )){
				not_iter->count++;
				continue;
			}

			check++;

		}
		//새로운
		if( check == notObject.size() ){
			testifyObject str;
			str.count = 0;
			str.p.x = comp_p.x;
			str.p.y = comp_p.y;
			notObject.push_back(str);

		}

		check =0;
	}

	if(countForLearning == LEARNING_COUNT-1)
		for(vector<testifyObject>::iterator not_iter = notObject.begin(); not_iter != notObject.end(); ){

			if(not_iter->count < NOTOBJECT_COUNT_STD)
				not_iter = notObject.erase(not_iter);
			else
				not_iter++;


		}

	countForLearning++;

}

void *Detector::AlramTimer(){

	while(true){
		if(isAlarming){
			sleep(5);
			isAlarming = false;


		}
	}
}
