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

void *Detector::BeginDectect(Queue *q, int frame_step)
{
	this->frame_step = frame_step;
	eventQ = q;
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

	return 0;
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

	int gr_thr = 60;
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

		lane = dl.beginDetectLine(&frame);

		Mat grayed_frame;
		cvtColor(frame,grayed_frame,CV_BGR2GRAY);
		equalizeHist(grayed_frame, grayed_frame);

		QueryPerformanceCounter((int64_t*)&start);

		// detect
		vector<Rect> found;
		detector.detectMultiScale(grayed_frame, found, scale_step, gr_thr, 0, min_obj_sz, max_obj_sz);

		// processing time (fps)

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
