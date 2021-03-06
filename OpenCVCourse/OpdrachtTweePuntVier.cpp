#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


Mat imgHSV, mask;
int hmin = 0, smin = 110, vmin = 153;
int hmax = 19, smax = 240, vmax = 255;
void main() {
	string path = "Resources/kerstballen.bmp";
	Mat img = imread(path);

	const string name = "Opdracht 2.4";
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	namedWindow(name, (640, 200));
	createTrackbar("Hue min", name, &hmin, 179);
	createTrackbar("Hue max", name, &hmax, 179);
	createTrackbar("Sat min", name, &smin, 255);
	createTrackbar("Sat max", name, &smax, 255);
	createTrackbar("Val min", name, &vmin, 255);
	createTrackbar("Val max", name, &vmax, 255);

	while (true) {

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		inRange(imgHSV, lower, upper, mask);

		imshow("Image Original", img);
		imshow("Image hsv", imgHSV);
		imshow("Image Mask", mask);
		waitKey(1);
	}
}