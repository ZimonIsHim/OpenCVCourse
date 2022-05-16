#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Basic Functions  //////////////////////

int threshold1 = 0;
int threshold2 = 0;

void main()
{
	string path = "Resources/fruit.jpg";
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

	const string name = "Opdracht 2.2";
	namedWindow(name);
	createTrackbar("Threshold 1", name, &threshold1, 100);
	createTrackbar("Threshold 2", name, &threshold2, 300);

	while (true)
	{
		cvtColor(img, imgGray, COLOR_BGR2GRAY);
		GaussianBlur(imgGray, imgBlur, Size(7, 7), 5, 0);
		Canny(imgBlur, imgCanny, threshold1, threshold2);

		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(imgCanny, imgDil, kernel);
		erode(imgDil, imgErode, kernel);

		imshow("Image", img);
		imshow("Image Gray", imgGray);
		imshow("Image Blur", imgBlur);
		imshow("Image Canny", imgCanny);
		imshow("Image Dilation", imgDil);
		imshow("Image Erode", imgErode);
		waitKey(1);
	}
}
