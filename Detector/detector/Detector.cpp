///////////////////////////////////////////////////////////////////////////
// OpenCV pedestrian detection examples.
// Written  by darkpgmr (http://darkpgmr.tistory.com), 2013


#include "../detector/Detector.h"





#define long long int int64

using namespace std;

static const unsigned usec_per_sec = 1000000;
static const unsigned usec_per_msec = 1000;

void detect_hog_inria(VideoCapture *vc);
void detect_hog_daimler(VideoCapture *vc);
void detect_hogcascades(VideoCapture *vc);
void detect_haarcascades(VideoCapture *vc);

struct timeval startTime, endTime;
struct timezone tz;

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

bool Detector::BeginDectect()
{


	VideoCapture *vc = NULL;


	vc = new VideoCapture("sample1.avi");
	if (!vc->isOpened())
	{
		cout << "can't open video file" << endl;
		return 0;
	}

	//select pedestrian detection method
	char method;
	cout << endl;
	cout << "  1. HOG (INRIA)\n"
		 << "  2. HOG (Daimler)\n"
		 << "  3. hogcascades\n"
		 << "  4. haarcascades\n";
	cout << "select detection method[1-4]: ";
	cin >> method;

	if(vc)
	{
		if(method=='1') detect_hog_inria(vc);
		if(method=='2') detect_hog_daimler(vc);
		if(method=='3') detect_hogcascades(vc);
		if(method=='4') detect_haarcascades(vc);
	}
	if(vc) delete vc;

	destroyAllWindows();

	return 0;
}

void Detector::detect_hog_inria(VideoCapture *vc)
{
	// detector (64x128 template)
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	// parameters
	double hit_thr = 0.1;
	double gr_thr = 2;

	Mat frame;
	int64 freq,start,finish;

	QueryPerformanceFrequency((int64_t*)&freq);
	while(1)
	{
		// input image
		*vc >> frame;
		if(frame.empty()) break;

		::QueryPerformanceCounter((int64_t*)&start);

		// detect
		vector<Rect> found;
        hog.detectMultiScale(frame, found, hit_thr, Size(32,32), Size(64,64), 1.11, gr_thr);

		// processing time (fps)
        ::QueryPerformanceCounter((int64_t*)&finish);

		double fps = freq / double(finish - start + 1);
		char fps_str[20];
		sprintf(fps_str, "FPS: %.1lf", fps);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0,255,0), 2);

		// draw results (bounding boxes)
		for(int i=0; i<(int)found.size(); i++)
			rectangle(frame, found[i], Scalar(0,255,0), 2);

		// display
		imshow("darkpgmr", frame);
		char ch = waitKey(10);
		if( ch == 27 ) break;				// ESC Key
		else if(ch == 32 )					// SPACE Key
		{
			while((ch = waitKey(10)) != 32 && ch != 27);
			if(ch == 27) break;
		}
	}
}

void Detector::detect_hog_daimler(VideoCapture *vc)
{
	// detector (48x96 template)
    HOGDescriptor hog(Size(48,96), Size(16,16), Size(8,8), Size(8,8), 9);
	hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());

	// parameters
	double hit_thr = 1.4;
	double gr_thr = 4;

	// run
	Mat frame;
	int64 freq,start,finish;
	QueryPerformanceFrequency((int64_t*)&freq);
	while(1)
	{
		// input image
		*vc >> frame;
		if(frame.empty()) break;

		::QueryPerformanceCounter((int64_t*)&start);

		// detect
		vector<Rect> found;
        hog.detectMultiScale(frame, found, hit_thr, Size(32,32), Size(64,64), 1.1, gr_thr);

		// processing time (fps)
		::QueryPerformanceCounter((int64_t*)&finish);
		double fps = freq / double(finish - start + 1);
		char fps_str[20];
		sprintf(fps_str, "FPS: %.1lf", fps);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0,255,0), 2);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0,255,0), 2);

		// draw results (bounding boxes)
		for(int i=0; i<(int)found.size(); i++)
			rectangle(frame, found[i], Scalar(0,255,0), 2);

		// display
		imshow("darkpgmr", frame);
		char ch = waitKey(10);
		if( ch == 27 ) break;				// ESC Key
		else if(ch == 32 )					// SPACE Key
		{
			while((ch = waitKey(10)) != 32 && ch != 27);
			if(ch == 27) break;
		}
	}
}

void Detector::detect_hogcascades(VideoCapture *vc)
{
	// detector (48x96 template)
	String cascadeName = "hogcascade_pedestrians.xml";
	CascadeClassifier detector(cascadeName);

/*
	if( !detector.load( cascadeName ) )
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return;
	}
*/

	// parameters
	int gr_thr = 6;
	double scale_step = 1.1;
	Size min_obj_sz(48,96);
	Size max_obj_sz(100,200);

	// run
	Mat frame;
	int64 freq,start,finish;
	::QueryPerformanceFrequency((int64_t*)&freq);
	while(1)
	{
		// input image
		*vc >> frame;
		if(frame.empty()) break;

		::QueryPerformanceCounter((int64_t*)&start);

		// detect
		vector<Rect> found;
		detector.detectMultiScale(frame, found, scale_step, gr_thr, 0, min_obj_sz, max_obj_sz);

		// processing time (fps)
		::QueryPerformanceCounter((int64_t*)&finish);
		double fps = freq / double(finish - start + 1);
		char fps_str[20];
		sprintf(fps_str, "FPS: %.1lf", fps);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0,255,0), 2);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0,255,0), 2);

		// draw results (bounding boxes)
		for(int i=0; i<(int)found.size(); i++)
			rectangle(frame, found[i], Scalar(0,255,0), 2);

		// display
		imshow("darkpgmr", frame);
		char ch = waitKey(10);
		if( ch == 27 ) break;				// ESC Key
		else if(ch == 32 )					// SPACE Key
		{
			while((ch = waitKey(10)) != 32 && ch != 27);
			if(ch == 27) break;
		}
	}
}

void Detector::detect_haarcascades(VideoCapture *vc)
{
	// detector (14x28 template)
	string cascadeName = "haarcascade_fullbody.xml";
	CascadeClassifier detector;
	if( !detector.load( cascadeName ) )
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return;
	}

	// parameters
	int gr_thr = 8;
	double scale_step = 1.05;
	Size min_obj_sz(40,100);
	Size max_obj_sz(65,170);

	// run
	Mat frame;
	int64 freq,start,finish;
	QueryPerformanceFrequency((int64_t*)&freq);
	while(1)
	{
		// input image
		*vc >> frame;
		if(frame.empty()) break;

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
		imshow("darkpgmr", frame);
		char ch = waitKey(10);
		if( ch == 27 ) break;				// ESC Key
		else if(ch == 32 )					// SPACE Key
		{
			while((ch = waitKey(10)) != 32 && ch != 27);
			if(ch == 27) break;
		}
	}
}
