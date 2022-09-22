#if 1
#include "opencv2/opencv.hpp"
#include <iostream>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;
using namespace std;
Point2f srcQuad[4];
Point2f dstQuad[4];
bool dragScr[4];
Point ptO1d;
Mat src;
Mat drawROI(Mat& img, Point2f* corners) {
	Mat cpy;
	img.copyTo(cpy);
	for (int i = 0; i < 4; i++) {
		circle(cpy, corners[i], 24, Scalar(192, 192, 255), -1, LINE_AA);
	}
	line(cpy, corners[0], corners[1], Scalar(128, 128, 255), 2, LINE_AA);
	line(cpy, corners[1], corners[2], Scalar(128, 128, 255), 2, LINE_AA);
	line(cpy, corners[2], corners[3], Scalar(128, 128, 255), 2, LINE_AA);
	line(cpy, corners[3], corners[0], Scalar(128, 128, 255), 2, LINE_AA);
	Mat disp;
	addWeighted(img, 0.3, cpy, 0.7, 0, disp);
	return disp;
}
void on_mouse(int event, int x, int y, int flags, void*)
{
	switch (event) {
	case EVENT_LBUTTONDOWN:
		for (int i = 0; i < 4; i++) {
			Point2f p(x, y);
			if (norm(srcQuad[i] - p) < 25) {
				dragScr[i] = true;
				ptO1d = Point(x, y);
			}
		}
		break;
	case EVENT_LBUTTONUP:
		for (int i = 0; i < 4; i++)
			dragScr[i] = false;
		break;
	case EVENT_MOUSEMOVE:
		for (int i = 0; i < 4; i++) {
			if (dragScr[i]) {
				int dx = x - ptO1d.x;
				int dy = y - ptO1d.y;
				srcQuad[i].x += dx;
				srcQuad[i].y += dy;
				Mat cpy;
				cpy = drawROI(src, srcQuad);
				imshow("img", cpy);
				ptO1d.x = x;
				ptO1d.y = y;
			}
		}
		break;
	}
}
int main()
{
	CV_EXPORTS_W int waitkey(int delay = 0);
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	VideoCapture cap(0);

	int cam_mode = 0;
	int key;

	if (!cap.isOpened()) {
		cerr << " Camera open failed! " << endl;
		return -1;
	}

	Mat frame;

	while (true) {
		cap >> frame;

		imshow("frame", frame);

		key = waitKey(10);

		if (key == 's')
			imwrite("고릴라.bmp", frame);


		if (key == 27)
			break;
		else if (key == 'i') {
			cam_mode++;
			if (cam_mode == 4)
				cam_mode = 0;
		}
		else if (key == 'j') {
			//Mat temp = imread("고릴라.bmp");
			//imshow("고릴라", temp);

			src = imread("고릴라.bmp");

			if (src.empty()) {
				cerr << "Image load failed!!" << endl;
				return -1;
			}
			int h = src.rows;
			int w = src.cols;
			int dw = 500;
			int dh = cvRound(dw * 297 / 210);

			srcQuad[0] = Point2f(30, 30);
			srcQuad[1] = Point2f(30, h - 30);
			srcQuad[2] = Point2f(w - 30, h - 30);
			srcQuad[3] = Point2f(w - 30, 30);

			dstQuad[0] = Point2f(0, 0);
			dstQuad[1] = Point2f(0, dh - 1);
			dstQuad[2] = Point2f(dw - 1, dh - 1);
			dstQuad[3] = Point2f(dw - 1, 0);

			dragScr[0] = false;
			dragScr[1] = false;
			dragScr[2] = false;
			dragScr[3] = false;

			Mat disp = drawROI(src, srcQuad);
			imshow("img", disp);
			setMouseCallback("img", on_mouse);
			while (true) {
				int key = waitKey();
				if (key == 13) {
					//destroyWindow("img");
					break;
				}
				else if (key == 27) {
					destroyAllWindows();
					return 0;
				}
			}
			Mat pers;
			pers = getPerspectiveTransform(srcQuad, dstQuad);
			Mat dst;
			warpPerspective(src, dst, pers, Size(dw, dh), INTER_CUBIC);
			imshow("dst", dst);
		}

	}
	destroyAllWindows();


	return 0;
}
#endif
