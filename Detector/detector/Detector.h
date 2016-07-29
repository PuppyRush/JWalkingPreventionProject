/////////////////////////////////////////////////////////////////////
// use_opencv.h
// written  by darkpgmr (http://darkpgmr.tistory.com), 2013

#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/video/video.hpp"

#include "opencv2/videostab.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"

#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include "DetectLine.h"
#include "../Queue.h"


struct THREAD_DETECTOR_BEGIN_PARAMETER{

	void* context;
	Queue *q;
	int frame_step;

};

template<class T>
class TypedMat
{
	T** m_pData;
	int m_nChannels;
	int m_nRows, m_nCols;

public:
	TypedMat():m_pData(NULL),m_nChannels(1),m_nRows(0),m_nCols(0){}
	~TypedMat(){if(m_pData) delete [] m_pData;}

	// OpenCV Mat ���� (�޸� ����)
	void Attach(const cv::Mat& m);
	void Attach(const IplImage& m);
	TypedMat(const cv::Mat& m):m_pData(NULL),m_nChannels(1),m_nRows(0),m_nCols(0) { Attach(m);}
	TypedMat(const IplImage& m):m_pData(NULL),m_nChannels(1),m_nRows(0),m_nCols(0) { Attach(m);}
	const TypedMat & operator =(const cv::Mat& m){ Attach(m); return *this;}
	const TypedMat & operator =(const IplImage& m){ Attach(m); return *this;}

	// ��(row) ��ȯ
	T* GetPtr(int r)
	{ assert(r>=0 && r<m_nRows); return m_pData[r];}

	// ������ ��ø (��������) -- 2D
	T * operator [](int r)
	{ assert(r>=0 && r<m_nRows); return m_pData[r];}

	const T * operator [](int r) const
	{ assert(r>=0 && r<m_nRows); return m_pData[r];}

	// ������ ��ø (��������) -- 3D
	T & operator ()(int r, int c, int k)
	{ assert(r>=0 && r<m_nRows && c>=0 && c<m_nCols && k>=0 && k<m_nChannels); return m_pData[r][c*m_nChannels+k];}

	const T operator ()(int r, int c, int k) const
	{ assert(r>=0 && r<m_nRows && c>=0 && c<m_nCols && k>=0 && k<m_nChannels); return m_pData[r][c*m_nChannels+k];}
};

template<class T>
void TypedMat<T>::Attach(const cv::Mat& m)
{
	assert(sizeof(T)==m.elemSize1());

	m_nChannels = m.channels();
	m_nRows = m.rows;
	m_nCols = m.cols;
	
	if(m_pData) delete [] m_pData;
	m_pData = new T * [m_nRows];
	for(int r=0; r<m_nRows; r++)
	{
		m_pData[r] = (T *)(m.data + r*m.step);
	}
}

template<class T>
void TypedMat<T>::Attach(const IplImage& m)
{
	assert(sizeof(T)==m.nSize);

	m_nChannels = m.nChannels;
	m_nRows = m.height;
	m_nCols = m.width;
	
	if(m_pData) delete [] m_pData;
	m_pData = new T * [m_nRows];
	for(int r=0; r<m_nRows; r++)
	{
		m_pData[r] = (T *)(m.imageData + r*m.widthStep);
	}
}

using namespace cv;


class Detector{

	private:
		Queue *eventQ;
		int frame_step;
		bool isActivitedCam;
		String filePath;
		string windowName;
		static const unsigned usec_per_sec = 1000000;
		static const unsigned usec_per_msec = 1000;
		struct timeval startTime, endTime;
		struct timezone tz;


	public:

		Detector(String filepath){filePath = filepath; isActivitedCam = false; windowName = "Detector"; eventQ	= NULL;}
		Detector(){isActivitedCam = true; windowName = "Detector"; eventQ = NULL;}
		~Detector(){;}

		bool QueryPerformanceFrequency(int64_t *frequency);
		bool QueryPerformanceCounter(int64_t *performance_count);
		void *BeginDectect(Queue *q, int );
		static void* getBeginDectect(void* th){
			THREAD_DETECTOR_BEGIN_PARAMETER *str = (THREAD_DETECTOR_BEGIN_PARAMETER *)th;
			return ( (Detector *)str->context)->BeginDectect(str->q, str->frame_step);
		}

		bool detect_haarcascades(VideoCapture *vc);

};