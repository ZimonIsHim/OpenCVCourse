#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	string path = "Resources/flowers.jpg";

	// Get original image.
	Mat img = imread(path);

	// Resize the image.
	Mat imgResized;
	resize(img, imgResized, Size(640, 480));

	// Crop the image.
	Mat imgCropped = img(Rect(0, 0, 340, 280));

	// Show images.
	imshow("Original Image", img);
	imshow("Resized Image", imgResized);
	imshow("Cropped Image", imgCropped);

	waitKey(0);
}
