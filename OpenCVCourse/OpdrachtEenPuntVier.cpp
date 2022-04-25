#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main() {

	string path = "Resources/mousepad.jpg";
	Mat img = imread(path);
	Mat matrix, imgWarp;
	float w = 250, h = 350;

	Point2f src[4] = { {496,1196},{1938,814},{1034,3084},{2765,2453} };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	for (int i = 0; i < 4; i++)
	{
		circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
	}

	Mat imgResized;
	resize(img, imgResized, Size(), 0.2, 0.2);

	imshow("Image Resized", imgResized);
	imshow("Image Warped", imgWarp);
	waitKey(0);

}