#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	string path = "Resources/ballstripes.png";
	Mat img = imread(path);
	Mat imgClose;

	Mat kernel = getStructuringElement(MORPH_RECT, Size(4, 4));

	morphologyEx(img, imgClose, MORPH_CLOSE, kernel);

	imshow("Image Original", img);
	imshow("Image Closed", imgClose);
	waitKey(0);
}
