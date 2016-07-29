/*
 * DetectLine.h
 *
 *  Created on: 2016. 7. 28.
 *      Author: cmk
 */

#ifndef DETECTOR_DETECTOR_DETECTLINE_H_
#define DETECTOR_DETECTOR_DETECTLINE_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <cv.h>
#include <algorithm>
#include <string>


using namespace cv;
using namespace std;

struct LANE{

	int begin_x;
	int begin_y;
	int end_x;
	int end_y;
	Mat *frame;
};

struct STRUCT_FOR_LINE{
	vector<Point> lines;
	int height;
	int avg_x;
};

class CompareOfContours : std::unary_function< vector<Point>, bool >
{
public:
    bool operator()(const vector<Point>& lhs, const vector<Point>& rhs) const
    {
        return lhs.size() > rhs.size();
    }
};

class CompareOfPointY : std::unary_function< Point, bool >
{
public:
    bool operator()(const Point& lhs, const Point& rhs) const
    {
        return lhs.y > rhs.y;
    }
};

class CompareOfPointX : std::unary_function< Point, bool >
{
public:
    bool operator()(const Point& lhs, const Point& rhs) const
    {
        return lhs.x > rhs.x;
    }
};

/*class CompareOfVertical : std::unary_function<vector<Point>, bool >
{
public:
    bool operator()(const vector<Point>& lhs, const vector<Point>& rhs) const
    {
		std::sort_heap(lhs.begin(),lhs.end() , CompareOfPointY() );
		std::sort_heap(rhs.begin(),rhs.end() , CompareOfPointY() );

		return lhs.at(0).y - lhs.at( lhs.size()-1 ).y > rhs.at(0).y - rhs.at( lhs.size()-1 ).y ? true : false;
    }
};*/
/*
class CompareOfVerticals : std::unary_function< STRUCT_FOR_LINE, bool >
{
public:
    bool operator()(const STRUCT_FOR_LINE& lhs,const STRUCT_FOR_LINE& rhs) const
    {
		std::sort_heap(lhs.lines.begin(),lhs.lines.end() , CompareOfPointY() );
		std::sort_heap(rhs.lines.begin(),rhs.lines.end() , CompareOfPointY() );

		lhs.height = lhs.lines.at(0).y - lhs.lines.at( lhs.lines.size()-1).y;
		rhs.height = rhs.lines.at(0).y - rhs.lines.at( rhs.lines.size()-1).y;

     return lhs.height > rhs.height ? true : false;
    }
};*/

class DetectLine{

	public:

		int get_indexOfLongestLane(vector<vector<Point> > *);
		LANE* beginDetectLine(Mat*);

	public:


};


#endif /* DETECTOR_DETECTOR_DETECTLINE_H_ */
