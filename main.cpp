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
	Mat current;
	VideoCapture capture;
	capture.open(CAP_ANY);
	namedWindow("III");
	capture.read(current);
	cvSetMouseCallback("III", onMouse, 0);

	int n = 0;
	bool initialized = false;
	Mat hist, weight;
	double sum = 0;
	while (true) {
		cout << "Loop: " << n++ << endl;
		capture.read(current);

		rectangle(current, cvPoint(drawing_box.x, drawing_box.y), cvPoint(drawing_box.x + drawing_box.width, drawing_box.y + drawing_box.height), CV_RGB(255, 0, 0), 2);
		imshow("III", current);
		if(waitKey(100) == 27)
			break;
	}
	current.release();
	capture.release();
	destroyWindow("III");
	return 0;
}
