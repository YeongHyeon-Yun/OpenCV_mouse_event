#if 0
#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/core/utils/logger.hpp"

using namespace cv;
using namespace std;

void affine_transform();
void affine_translation();
void affine_shear();
void affine_scale();
void affine_rotation();
void affine_flip();

int main(void)
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	//affine_transform();
	//affine_translation();
	//affine_shear();
	//affine_scale();
	//affine_rotation();
	affine_flip();

	cout << "opencv version: " << CV_VERSION << endl;
	return 0;
}

void affine_transform()
{
	Mat src = imread("tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	Point2f srcPts[3], dstPts[3];
	srcPts[0] = Point2f(0, 0);
	srcPts[1] = Point2f(src.cols - 1, 0);
	srcPts[2] = Point2f(src.cols - 1, src.rows - 1);

	dstPts[0] = Point2f(50, 50);
	dstPts[1] = Point2f(src.cols - 100, 100);
	dstPts[2] = Point2f(src.cols - 50, src.rows - 50);

	Mat M = getAffineTransform(srcPts, dstPts);   // Affine 변환 함수
	//cout << "M:  " << M << endl;

	Mat dst;
	warpAffine(src, dst, M, Size());  // Affine 변환 함수를 적용시킨 객체를 이용하여 이미지에 적용

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void affine_translation()
{
	Mat src = imread("tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	Mat M = Mat_<double>({ 2, 3 }, { 1, 0, 150, 0, 1, 100 });
	Mat dst;

	warpAffine(src, dst, M, Size(src.cols + 170, src.rows + 120), 1, 0, Scalar(255, 0, 255));
	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void affine_shear()
{
	Mat src = imread("tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	double mx = 0.3;
	Mat M = Mat_<double>({ 2, 3 }, { 1, mx, 0, 0, 1, 0 });

	Mat dst;
	warpAffine(src, dst, M, Size(cvRound(src.cols + src.rows * mx), src.rows));

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();
}

void affine_scale()
{
	Mat src = imread("rose.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	Mat dst1, dst2, dst3, dst4;
	resize(src, dst1, Size(), 4, 4, INTER_NEAREST);
	resize(src, dst2, Size(1920, 1280));
	resize(src, dst3, Size(1920, 1280), 0, 0, INTER_CUBIC);
	resize(src, dst4, Size(1920, 1280), 0, 0, INTER_LANCZOS4);

	imshow("src", src);
	imshow("dst1", dst1(Rect(400, 500, 400, 400)));
	imshow("dst2", dst2(Rect(400, 500, 400, 400)));
	imshow("dst3", dst3(Rect(400, 500, 400, 400)));
	imshow("dst4", dst4(Rect(400, 500, 400, 400)));

	waitKey();
	destroyAllWindows();
}

void affine_rotation()
{
	Mat src = imread("tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	Point2f cp(src.cols / 2.f, src.rows / 2.f);
	Mat M = getRotationMatrix2D(cp, 20, 1);   // 20이 양수면 반시계 , 음수면 시계방향

	Mat dst;
	warpAffine(src, dst, M, Size());

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();
}

void affine_flip()
{
	Mat src = imread("eastsea.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	imshow("src", src);

	Mat dst;
	int flipCode[] = { 1, 0, -1 };
	for (int i = 0; i < 3; i++) {
		flip(src, dst, flipCode[i]);

		String desc = format("flipCode: %d", flipCode[i]);
		putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 1, LINE_AA);

		imshow("dst", dst);
		waitKey();
	}

	destroyAllWindows();
}

#endif


#if 0
#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/core/utils/logger.hpp"

using namespace cv;
using namespace std;

Mat src;
Point2f srcPts[4], dstPts[4];

void on_mouse(int event, int x, int y, int flags, void* userdata);

int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	src = imread("card.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow("src");
	setMouseCallback("src", on_mouse);

	imshow("src", src);
	waitKey();

	return 0;
}

void on_mouse(int event, int x, int y, int flags, void*)
{
	static int cnt = 0;

	if (event == EVENT_LBUTTONDOWN) {
		if (cnt < 4) {
			srcPts[cnt++] = Point2f(x, y);

			circle(src, Point(x, y), 5, Scalar(0, 0, 255), -1);
			imshow("src", src);

			if (cnt == 4) {
				int w = 200, h = 300;

				dstPts[0] = Point2f(0, 0);
				dstPts[1] = Point2f(w - 1, 0);
				dstPts[2] = Point2f(w - 1, h - 1);
				dstPts[3] = Point2f(0, h - 1);

				Mat pers = getPerspectiveTransform(srcPts, dstPts);

				Mat dst;
				warpPerspective(src, dst, pers, Size(w, h));

				imshow("dst", dst);
			}
		}
	}
}

#endif



#if 0
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;
using namespace std;
Point2f srcQuad[4];
Point2f dstQuad[4];
bool dragScr[4];
Point ptOld;
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
void onMouse(int event, int x, int y, int flags, void*) {
	switch (event) {
	case EVENT_LBUTTONDOWN:
		for (int i = 0; i < 4; i++) {
			Point2f p(x, y);
			if (norm(srcQuad[i] - p) < 25) {
				dragScr[i] = true;
				ptOld = Point(x, y);
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
				int dx = x - ptOld.x;
				int dy = y - ptOld.y;
				srcQuad[i].x += dx;
				srcQuad[i].y += dy;
				Mat cpy;
				cpy = drawROI(src, srcQuad);
				imshow("img", cpy);
				ptOld.x = x;
				ptOld.y = y;
			}
		}
		break;
	}
}
int main()
{
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	src = imread("scanned.jpg");
	if (src.empty()) {
		cerr << "Image load failed!" << endl;
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
	setMouseCallback("img", onMouse);
	while (true) {
		int key = waitKey();
		if (key == 13)
			break;
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
	waitKey();
	destroyAllWindows();
	return 0;
}
#endif


// 포인트 4개 찍어서 찍은 이미지 평평하게 펴주기 짱신기
#if 0
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
	utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	src = imread("scanned.jpg");
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
		if (key == 13)
			break;
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
	waitKey();
	destroyAllWindows();
	return 0;
}
#endif


//  ------------------------------  과제  ---------------------------------------

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










//   video 함수 코드
#if 0
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/utils/logger.hpp>

using namespace std;
using namespace cv;
Mat cartoon_filter(Mat img)
{
	int h, w;
	Mat img2;
	Mat blr;
	Mat edge;
	Mat dst;
	h = img.rows;
	w = img.cols;

	resize(img, img2, Size(w / 2, h / 2));
	bilateralFilter(img2, blr, -1, 20, 7);
	Canny(img2, edge, 80, 120);
	cvtColor(edge, edge, COLOR_GRAY2BGR);

	bitwise_and(blr, edge, dst);
	resize(dst, dst, Size(w, h), INTER_NEAREST);
	return dst;
}

Mat pencil_sketch(Mat img)
{
	Mat gray;
	Mat blr;
	Mat dst;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, blr, Size(), 10);
	divide(gray, blr, dst, 255);
	return dst;
}

int main()
{
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
		if (frame.empty())
			break;

		if (cam_mode == 1)
			frame = cartoon_filter(frame);

		else if (cam_mode == 2)
			frame = pencil_sketch(frame);

		else if (cam_mode == 3)
			frame = ~frame;

		imshow("frame", frame);

		key = waitKey(10);
		if (key == 27)
			break;
		else if (key == 'i') {
			cam_mode++;
			if (cam_mode == 4)
				cam_mode = 0;
		}
	}
	destroyAllWindows();

}

#endif




