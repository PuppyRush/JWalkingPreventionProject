

#include "DetectLine.h"


LANE* DetectLine::beginDetectLine(Mat *frame){


	LANE* lane = new LANE();
	Mat src_gray;

	cvtColor( *frame, src_gray, CV_BGR2GRAY );
	blur( src_gray, src_gray, Size(3,3) );

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	/// Detect edges using canny
	Canny( src_gray, canny_output, 150, 150*2, 3 );
	/// Find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Draw contours
	Mat resultFrame = Mat::zeros( canny_output.size(), CV_8UC3 );


	std::sort_heap(contours.begin(), contours.end(), CompareOfContours());

	int laneNumber = get_indexOfLongestLane(&contours);
	int size=contours.at(laneNumber).size();

	for(int l=0 ; l < size-1 ; l++){
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			line(*frame,contours.at(laneNumber).at(l), contours.at(laneNumber).at(l+1), Scalar(255,255,255),2,8);
		}

	lane->begin_x = contours.at(laneNumber).at(0).x;
	lane->begin_y = contours.at(laneNumber).at(0).y;
	lane->end_x = contours.at(laneNumber).at(size-1).x;
	lane->end_y = contours.at(laneNumber).at(size-1).y;
	lane->frame = frame;

	return lane;


}




int DetectLine::get_indexOfLongestLane(vector<vector<Point> > *ary){

	int findedIndex=0;
	int tempHeight=0;
	int tempWidth=0;
	int left_of_x_average=0;
	long x_sum=0;
	long x_avg=0;
	tempWidth = 500000;

/*
	vector<STRUCT_FOR_LINE> line_vc;
	for(int i=0 ; i < ary->size() ; i++){
		STRUCT_FOR_LINE str;
		str.height=0;
		str.avg_x=0;
		str.lines = ary->at(i);
		line_vc.push_back(str);
	}


	return line_vc.at(0).lines;
*/

	for(int i=0 ; i < ary->size()-1 ; i++){

		std::sort_heap(ary->at(i).begin(), ary->at(i).end() , CompareOfPointY() );


		int size = ary->at(i).size();
		int height = ary->at(i).at(0).y -  ary->at(i).at(size-1).y;
		if(tempHeight < height){
			tempHeight = height;
			findedIndex = i;

		}


	}

/*	tempWidth = 500000;
	for(int i=0 ; i < 5 ; i++){

		for(int l=0 ; l < ary->at(i).size() ; l++){
			x_sum+=ary->at(i).at(l).x;
		}
		x_avg=x_sum/ary->at(i).size();
		if(x_avg < tempWidth){
			tempWidth = x_avg;
			findedIndex = i;
		}
		x_sum=0;


	}*/


	return findedIndex;
}
