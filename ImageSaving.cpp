#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

bool mouseIsDragging;
bool mouseMove;
bool rectangleSelected;
cv::Point initialClickPoint, currentMousePoint;
cv::Rect rectangleROI;
int xnach, xkon, ynach, ykon;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	Mat* videoFeed = (Mat*)userdata;

	if (event == CV_EVENT_LBUTTONDOWN && mouseIsDragging == false)
	{
		//ѕолучаем начальную точку.
		initialClickPoint = cv::Point(x, y);
		xnach = x;
		ynach = y;
		//пользователь начинает перемещать мышку.
		mouseIsDragging = true;
	}
	/* user is dragging the mouse */
	if (event == CV_EVENT_MOUSEMOVE && mouseIsDragging == true)
	{
		//получаем текущую точку.
		currentMousePoint = cv::Point(x, y);
		//пользователь, нажав мышку, перемещает ее.
		mouseMove = true;
	}
	/* user has released left button */
	if (event == CV_EVENT_LBUTTONUP && mouseIsDragging == true)
	{
		//задан пр€моугольник »ќ– на пр€моугольник, который выбрал пользователь.
		rectangleROI = Rect(initialClickPoint, currentMousePoint);
		xkon = x;
		ykon = y;
		int temp;
		if (xnach > xkon)
		{
			temp = xnach;
			xnach = xkon;
			xkon = temp;
		}
		if (ynach > ykon)
		{
			temp = ynach;
			ynach = ykon;
			ykon = temp;
		}
		//сброс логических переменных.
		mouseIsDragging = false;
		mouseMove = false;
		rectangleSelected = true;
	}
}

int main(int argc, char* argv[])
{
	VideoCapture cap(0); // открывает видеокамеру.

	if (!cap.isOpened())  // если не открываетс€, закрываетс€ программа.
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //получаем ширину.
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //получаем высоту.

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //создаем окно.

	while (1)
	{
		Mat frame;



		bool bSuccess = cap.read(frame); // читаем новые кадры дл€ видео.

		if (!bSuccess) //если не получаетс€, то отмен€ем.
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		if (mouseMove == false && rectangleSelected == true)
		{
			IplImage image((IplImage)frame);
			IplImage* img = &image;
			IplImage* im = &image;
			//сброс rectangleSelected так, пользователь может выбрать другую область при необходимости.
			rectangleSelected = false;
			CvRect cropRect = cvRect(xnach, ynach, xkon - xnach, ykon - ynach);
			cvSetImageROI(img, cropRect);
			cvCopy(img, im, NULL);
			cvSaveImage("my.jpeg", img);
			cout << "Left mouse button is clicked -  position (" << xnach << ", " << ynach << ")" << endl;
			cout << "Left mouse button is clicked -  position (" << xkon << ", " << ykon << ")" << endl;
			cvResetImageROI(img);
		}
		if (mouseMove == true)
		{
			rectangle(frame, initialClickPoint, cv::Point(currentMousePoint.x, currentMousePoint.y), cv::Scalar(0, 255, 0), 1, 8, 0);
		}
		imshow("MyVideo", frame); 
		setMouseCallback("MyVideo", CallBackFunc, NULL);
		if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}