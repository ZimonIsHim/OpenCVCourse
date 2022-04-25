#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// void main()
// {
// 	string path = "Resources/test.png";
// 	Mat img = imread(path);
//
// 	// Resize the image.
// 	Mat imgResized;
// 	// cout << img.size() << endl;
// 	// resize(img, imgResized, Size(640, 480));
// 	resize(img, imgResized, Size(), 0.5, 0.5);
//
// 	// Crop the image.
// 	Mat imgCrop;
// 	Rect roi(100, 100, 300, 250);
// 	imgCrop = img(roi);
//
// 	imshow("Image", img);
// 	imshow("ImageResized", imgResized);
// 	imshow("ImageCrop", imgCrop);
//
// 	waitKey(0);
// }
