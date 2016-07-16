
/*
================================================================================
Reza Adhitya Saputra
radhitya@uwaterloo.ca
================================================================================
*/

#ifndef OPENCV_WRAPPER
#define OPENCV_WRAPPER

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <unordered_map>

#include "AVector.h"

/*
enum ImageType
{
BGR255 = CV_8UC3,
};*/

#define BGR_255 CV_8UC3

/*
================================================================================
================================================================================
*/
struct MyColor
{
public:
	int _r;
	int _g;
	int _b;

	MyColor(int r, int g, int b)
	{
		this->_r = r;
		this->_g = g;
		this->_b = b;
	}

	MyColor(int val)
	{
		this->_r = val;
		this->_g = val;
		this->_b = val;
	}
};

/*
================================================================================
wrapper for cv::Mat
================================================================================
*/
struct CVImg
{
public:

	cv::Mat _img;
	bool _isColor;
	std::string _name;

	CVImg(std::string name)
	{
		this->_name = name;
	}

	// show
	void Show()
	{
		cv::namedWindow(_name, cv::WINDOW_AUTOSIZE);
		cv::imshow(_name, _img);
	}

	// color
	void LoadColor(std::string filename)
	{
		_img = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		_isColor = true;
	}

	// grayscale
	void LoadGrayscale(std::string filename)
	{
		_img = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		_isColor = false;
	}

	void CreateColorImage(int height, int width)
	{
		_img = cv::Mat::zeros(height, width, CV_8UC3);
	}

	void CreateColorImage(int squareSize)
	{
		_img = cv::Mat::zeros(squareSize, squareSize, CV_8UC3);
	}

	void SetColorPixel(int x, int y, MyColor color)
	{
		_img.at<cv::Vec3b>(y, x)[0] = color._b; // blue
		_img.at<cv::Vec3b>(y, x)[1] = color._g; // green
		_img.at<cv::Vec3b>(y, x)[2] = color._r; // red
	}

	int GetGrayValue(int x, int y)
	{
		return (int)_img.at<uchar>(y, x);
	}

	int GetColorValue(int x, int y, int channel)
	{
		return _img.at<cv::Vec3b>(y, x)[channel];
	}

	int GetRows()
	{
		return _img.rows;
	}

	int GetCols()
	{
		return _img.cols;
	}

};

/*
================================================================================
OpenCV Rendering
================================================================================
*/
class OpenCVWrapper
{
public:
	cv::RNG _rng;

public:
	// to do: implement this
	std::unordered_map <std::string, cv::Mat> _images;

	OpenCVWrapper();
	~OpenCVWrapper();

	MyColor GetRandomColor();

	// to do: implement this
	void CreateImage(std::string imageName, int width, int height, int imgType);
	void ShowImage(std::string imageName);
	void SaveImage(std::string imageName, std::string filePath);

	void SetPixel(std::string imageName, int x, int y, MyColor col);
	int GetNumColumns(std::string imageName);
	int GetNumRows(std::string imageName);

	void WaitKey();

	// point polygon test
	template <typename T>
	float PointPolygonTest(std::vector<T> shape_contours, T pt);

	//void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
	template <typename T>
	void DrawLine(std::string imageName, T pt1, T pt2, MyColor color, int thickness, float scale = 1.0f);

	void PutText(std::string imageName, std::string text, AVector pos, MyColor col, float scale = 0.5f, float thickness = 1)
	{
		cv::Mat img = _images[imageName];
		cv::putText(img, text, cv::Point(pos.x, pos.y), cv::FONT_HERSHEY_SIMPLEX, scale, cv::Scalar(col._b, col._g, col._r), thickness);
	}

	// drawing

	void DrawCircle(std::string imageName, AVector pt, MyColor col, float radius)
	{
		cv::Mat drawing = _images[imageName];
		cv::circle(drawing, cv::Point(pt.x, pt.y), radius, cv::Scalar(col._b, col._g, col._r), -1);
	}

	std::vector<cv::Point2f> ConvertAVectorToCvPoint2f(std::vector<AVector> contour);

	template <typename T>
	void DrawPolyOnCVImage(cv::Mat img,
		std::vector<T> shape_contours,
		MyColor color,
		bool isClosed,
		float thickness = 1.0f,
		float scale = 1.0f,
		float xOffset = 0,
		float yOffset = 0);

	template <typename T>
	void DrawPoly(std::string imageName,
		std::vector<T> shape_contours,
		MyColor color,
		bool isClosed,
		float thickness = 1.0f,
		float scale = 1.0f,
		float xOffset = 0,
		float yOffset = 0);

	template <typename T>
	void DrawRetranslatedPoly(std::string imageName,
		std::vector<T> shape_contours,
		std::vector<T> medial_axis,
		MyColor color,
		float thickness = 1.0f,
		float scale = 1.0f);

	template <typename T>
	void DrawFilledPoly(std::string imageName,
		std::vector<T> shape_contours,
		MyColor color,
		float scale = 1.0f,
		float xOffset = 0,
		float yOffset = 0);
};

#endif