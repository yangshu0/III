#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

bool roiDefined = false;
CvRect drawing_box;

void onMouse(int event, int x, int y, int flags, void *param)
{
	static bool buttonDown = false;

	switch (event)
	{
		case CV_EVENT_LBUTTONDOWN:
			cout << "Button Down" << endl;
			roiDefined = false;
			buttonDown = true;
			//the left up point of the rect  
			drawing_box.x = x;
			drawing_box.y = y;
			drawing_box.width = 0;
			drawing_box.height = 0;
			break;
		case CV_EVENT_MOUSEMOVE:
			if (buttonDown) {
				drawing_box.width = x - drawing_box.x;
				drawing_box.height = y - drawing_box.y;
			}
			break;
		case CV_EVENT_LBUTTONUP:
			cout << "Button Up" << endl;
			buttonDown = false;
			drawing_box.width = x - drawing_box.x;
			drawing_box.height = y - drawing_box.y;
			roiDefined = true;
			//waitKey(10);
			break;
	}
	return;
}

int main(int argc, char* argv[])
{
	//VideoCapture capture;
	//capture.open(CAP_ANY);
	namedWindow("III");
	//capture.read(current);
	cvSetMouseCallback("III", onMouse, 0);

	int n = 0;
	bool initialized = false;
	Mat colorImage, yuvImage;
	double sum = 0;
	
	{
		cout << "Loop: " << n++ << endl;
		colorImage = imread("shadow.jpeg");
		cvtColor(colorImage, yuvImage, CV_BGR2YUV);
		vector<Mat> c3;
		split(yuvImage,c3);

		Mat l1 = Mat(colorImage.size(), CV_64FC1);
		Mat l2 = Mat(colorImage.size(), CV_64FC1);
		Mat theta = Mat(colorImage.size(), CV_64FC1);

		for (int r = 0; r < colorImage.rows; ++r) {
			for (int c = 0; c < colorImage.cols; ++c) {
				if (c3[1].at<uchar>(r,c) != 0) {
					l1.at<double>(r,c) = c3[2].at<uchar>(r,c) / c3[1].at<uchar>(r,c);
					l2.at<double>(r,c) = c3[0].at<uchar>(r,c) / c3[1].at<uchar>(r,c);
					theta.at<double>(r,c) = atan(l1.at<double>(r,c) / l2.at<double>(r,c));
				}
				else {
					l1.at<double>(r,c) = 0;
					l2.at<double>(r,c) = 0;	
					theta.at<double>(r,c) = 0;
				}
			}
		}
		
		Mat grayImage;
		cvtColor(colorImage, grayImage, CV_BGR2GRAY);


		for (int r = 0; r < grayImage.rows; ++r) {
			for (int c = 0; c < grayImage.cols; ++c) {
				grayImage.at<uchar>(r,c) *= sin(theta.at<double>(r,c));
			}
		}
		
		/*
		for (int r = 0; r < grayImage.rows; ++r) {
			for (int c = 0; c < grayImage.cols; ++c) {
				if (grayImage.at<uchar>(r,c) == 0) {
					for (int k = c; k < grayImage.cols; ++k) {
						if (grayImage.at<uchar>(r,k) != 0) {
							grayImage.at<uchar>(r,c) = grayImage.at<uchar>(r,k);
							break;
						}
					}
				}
			}
		}*/

		namedWindow("III");
		moveWindow("III", 100, 100);
		namedWindow("rrr");
		moveWindow("rrr", 400, 400);
		imshow("III", colorImage);
		imshow("rrr", grayImage);
		waitKey(0);
	}
	
	destroyWindow("rrr");
	destroyWindow("III");

	return 0;
}
