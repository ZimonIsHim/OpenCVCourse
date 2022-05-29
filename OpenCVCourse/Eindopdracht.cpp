#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

///////////////  Eindopdracht - Document Scanner  //////////////////////

Mat imgOriginal, imgGray, imgBlur, imgCanny, imgThre, imgDil, imgErode, imgWarp, imgCrop, imgLet, imgWords, imgLetters;
vector<Point> initialPoints, docPoints;
float w = 420 * 2, h = 596 * 2;

Mat preProcessing(Mat img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);

	return imgDil;
}

vector<Point> getContours(Mat image) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	vector<Point> biggest;
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);

		string objectType;

		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) {

				biggest = { conPoly[i][0],conPoly[i][1] ,conPoly[i][2] ,conPoly[i][3] };
				maxArea = area;
			}

		}
	}
	return biggest;
}


void detectWords(const Mat image) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int amountOfWords = 0;

	// connect horizontally oriented regions
	Mat connected;

	image.copyTo(connected);
	Mat morphKernel = getStructuringElement(MORPH_RECT, Size(3, 1));
	morphologyEx(image, connected, MORPH_CLOSE, morphKernel);
	// find contours
	Mat mask = Mat::zeros(image.size(), CV_8UC1);
	//imshow("Image connected Words", connected); // shows the b/w image used to find the words
	findContours(connected, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	//crash prevention in case the program can't find any contours
	if (contours.empty()) return;

	for (int i = 0; i >= 0; i = hierarchy[i][0]) {
		Rect rect = boundingRect(contours[i]);
		Mat maskROI(mask, rect);
		maskROI = Scalar(0, 0, 0);
		// fill the contour
		drawContours(mask, contours, i, Scalar(255, 255, 255), FILLED);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

		
		double r = (double)countNonZero(maskROI) / (rect.width * rect.height);

		if (r > .45 /* assume at least 45% of the area is filled if it contains text */
			&&
			(rect.height > 8 && rect.width > 8) /* constraints on region size */
			/* these two conditions alone are not very robust. better to use something
			like the number of significant peaks in a horizontal projection as a third condition */
			)
		{
			rectangle(imgWords, rect, Scalar(0, 255, 0), 2);
			amountOfWords++;
			putText(imgWords, to_string(amountOfWords), Point(rect.x, rect.y), FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255));

		}
	}
	cout << "words:" << amountOfWords++ << endl;

}

void detectLetters(const Mat image) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int amountOfLetters = 0;

	Mat connected;
	image.copyTo(connected);

	//Connect vertical oriented regions for better detection
	Mat morphKernel = getStructuringElement(MORPH_RECT, Size(1, 4));
	morphologyEx(connected, connected, MORPH_CLOSE, morphKernel);

	//find contours
	Mat mask = Mat::zeros(image.size(), CV_8UC1);
	imshow("Image connected Letters", connected);
	findContours(connected, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	//crash prevention in case the program can't find any contours
	if (contours.empty()) return;

	for (int i = 0; i >= 0; i = hierarchy[i][0]) {
		Rect rect = boundingRect(contours[i]);
		Mat maskROI(mask, rect);
		maskROI = Scalar(0, 0, 0);
		// fill the contour
		drawContours(mask, contours, i, Scalar(255, 255, 255), FILLED);
		

		
		double r = (double)countNonZero(maskROI) / (rect.width * rect.height);

		if (r > .3 /* assume at least 30% of the area is filled if it contains a letter */
			&&
			(rect.height > 16 && rect.width > 4) /* constraints on region size */
			)
		{
			//draw a rectangle around the letter and indicate the number
			rectangle(imgLetters, rect, Scalar(0, 255, 0), 2);
			amountOfLetters++;
			putText(imgLetters, to_string(amountOfLetters), Point(rect.x, rect.y), FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255));

		}
	}
	cout << "letters:" << amountOfLetters++ << endl;
	//downscale the image so it properly fit's in a screen
	pyrDown(imgLetters, imgLetters);

}

void detectWordsLetters(Mat image) {
	Mat small;

	//Upscale the image for more precise detection
	pyrUp(image, imgLetters);

	image.copyTo(imgWords);
	//convert image to black and white
	cvtColor(imgLetters, small, COLOR_BGR2GRAY);

	//morpholigical gradient transformation
	Mat grad;
	Mat morphKernel = getStructuringElement(MORPH_CROSS, Size(2, 3));
	morphologyEx(small, grad, MORPH_GRADIENT, morphKernel);
	Mat bw;
	threshold(grad, bw, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);

	//start calling the detect methods
	detectLetters(bw);

	//downscale the size of bw cause word detection works better with smaller images
	pyrDown(bw, bw);
	detectWords(bw);
	
}

void drawPoints(vector<Point> points, Scalar color)
{
	for (int i = 0; i < points.size(); i++)
	{
		circle(imgOriginal, points[i], 10, color, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
	}
}

vector<Point> reorder(vector<Point> points)
{
	vector<Point> newPoints;
	vector<int>  sumPoints, subPoints;

	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //1
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //2
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); //3

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
	Point2f src[4] = { points[0],points[1],points[2],points[3] };
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}

void main() {

	string path = "Resources/testdocument.jpg";
	imgOriginal = imread(path);

	// Preprpcessing - Step 1 
	imgThre = preProcessing(imgOriginal);

	// Get Contours - Biggest  - Step 2
	initialPoints = getContours(imgThre);
	docPoints = reorder(initialPoints);

	// Warp - Step 3 
	imgWarp = getWarp(imgOriginal, docPoints, w, h);

	//Crop - Step 4
	int cropVal = 5;
	Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
	imgCrop = imgWarp(roi);
	detectWordsLetters(imgCrop);

	imshow("Image", imgOriginal);
	imshow("Image Dilation", imgThre);
	imshow("Image Warp", imgWarp);
	imshow("Image Crop", imgCrop);
	imshow("Image letters", imgLetters);
	imshow("Image Words", imgWords);
	
	waitKey(0);

}