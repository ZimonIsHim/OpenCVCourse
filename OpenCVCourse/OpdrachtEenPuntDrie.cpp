#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	// Colors.
	const auto white(Scalar(255, 255, 255));
	const auto black(Scalar(0, 0, 0));
	const auto yellow(Scalar(0, 221, 254));
	const auto blue(Scalar(184, 87, 0));

	// Dimensions.
	constexpr int width = 512;
	constexpr int heigth = 512;


	Mat img(width, heigth, CV_8UC3, white);

	// Line.
	line(img, Point(0, 0), Point(width, heigth), black, 1);

	// Flag.
	rectangle(img, Rect(0, 0, 300, 200), blue, FILLED);
	rectangle(img, Rect(0, 200, 300, 200), yellow, FILLED);

	// Circle.
	circle(img, Point(width / 1.5, heigth / 1.5), 10, black, 1);

	// Text.
	putText(img, "Mir", Point(width / 2, heigth / 2), FONT_ITALIC, 0.75, 3);
	

	imshow("Image", img);
	waitKey(0);
}
