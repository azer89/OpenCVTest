
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
//#include "ImageThinning.h"

#include <unordered_map>

#include "AVector.h"

#define BGR_255 CV_8UC3		// blue red green
#define FLOAT_IMG CV_32FC1	// floating point

/*
================================================================================
================================================================================
*/
struct MyColor
{
public:
	int _r; int _g; int _b;

	MyColor(int r, int g, int b)
	{
		this->_r = r; this->_g = g; this->_b = b;
	}

	MyColor(int val)
	{
		this->_r = val; this->_g = val; this->_b = val;
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

	cv::Mat		_img;
	bool		_isColor;
	std::string _name;

public:

	// ===== constructors =====
	CVImg(std::string name) { this->_name = name; }
	CVImg() {}

	// get image
	cv::Mat GetCVImage() { return _img; }

	// ===== show =====
	void Show()
	{
		cv::namedWindow(_name, cv::WINDOW_AUTOSIZE);
		cv::imshow(_name, _img);
	}

	// ===== color =====
	void LoadColor(std::string filename)
	{
		_img = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		_isColor = true;
	}

	// ===== grayscale ===== 
	void LoadGrayscale(std::string filename)
	{
		_img = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		_isColor = false;
	}

	// ===== create =====
	void CreateGrayscaleImage(int height, int width) { _img = cv::Mat::zeros(height, width, CV_8UC1);  _isColor = true; }
	void CreateGrayscaleImage(int squareSize)		 { _img = cv::Mat::zeros(squareSize, squareSize, CV_8UC1);  _isColor = true; }
	void CreateColorImage(int height, int width)     { _img = cv::Mat::zeros(height, width, CV_8UC3);  _isColor = true; }
	void CreateColorImage(int squareSize)		     { _img = cv::Mat::zeros(squareSize, squareSize, CV_8UC3);  _isColor = true; }
	void CreateFloatImage(int height, int width)     { _img = cv::Mat::zeros(height, width, CV_32FC1); _isColor = false; }
	void CreateFloatImage(int squareSize)		     { _img = cv::Mat::zeros(squareSize, squareSize, CV_32FC1); _isColor = false; }
	void CreateIntegerImage(int height, int width)   { _img = cv::Mat::zeros(height, width, CV_32SC1); _isColor = false; }
	void CreateIntegerImage(int squareSize)		     { _img = cv::Mat::zeros(squareSize, squareSize, CV_32SC1); _isColor = false; } // signed integer

	// ===== set =====
	void SetFloatPixel(int x, int y, float val)  { _img.at<float>(y, x) = val; }

	void SetIntegerPixel(int x, int y, int val)  { _img.at<int>(y, x) = val; }

	void SetColorPixel(int x, int y, MyColor color)
	{
		_img.at<cv::Vec3b>(y, x)[0] = color._b; // blue
		_img.at<cv::Vec3b>(y, x)[1] = color._g; // green
		_img.at<cv::Vec3b>(y, x)[2] = color._r; // red
	}

	void SetGrayValue(int x, int y, int val) { _img.at<uchar>(y, x) = val; }

	// ===== get =====
	float GetFloatValue(int x, int y) { return _img.at<float>(y, x); }

	int   GetIntegerValue(int x, int y) { return _img.at<int>(y, x); }

	int   GetGrayValue(int x, int y) { return (int)_img.at<uchar>(y, x); }

	int   GetColorValue(int x, int y, int channel) { return _img.at<cv::Vec3b>(y, x)[channel]; }

	// row
	int GetRows() { return _img.rows; }

	// column
	int GetCols() { return _img.cols; }

	void SaveImage(std::string filename) { cv::imwrite(filename, _img); }

	void SaveDistanceImage(std::string filename)
	{
		// min max
		double minVal; double maxVal;
		cv::minMaxLoc(_img, &minVal, &maxVal, 0, 0, cv::Mat());
		minVal = abs(minVal); maxVal = abs(maxVal);

		// drawing
		cv::Mat drawing = cv::Mat::zeros(_img.size(), CV_8UC3);

		for (int j = 0; j < drawing.rows; j++)
		{
			for (int i = 0; i < drawing.cols; i++)
			{
				if (_img.at<float>(j, i) < 0)
				{
					drawing.at<cv::Vec3b>(j, i)[0] = 255 - (int)abs(_img.at<float>(j, i)) * 255 / minVal;
				}
				else if (_img.at<float>(j, i) > 0)
				{
					drawing.at<cv::Vec3b>(j, i)[2] = 255 - (int)_img.at<float>(j, i) * 255 / maxVal;
				}
				else
				{
					drawing.at<cv::Vec3b>(j, i)[0] = 255;
					drawing.at<cv::Vec3b>(j, i)[1] = 255;
					drawing.at<cv::Vec3b>(j, i)[2] = 255;
				}
			}
		}

		// save image
		imwrite(filename, drawing);
	}
};


/*
================================================================================
================================================================================
*/
class OpenCVWrapper
{
public:
	cv::RNG _rng;
	std::unordered_map <std::string, cv::Mat> _images;

public:
	// constructor	
	OpenCVWrapper();

	// destructor
	~OpenCVWrapper();

	// signed distance
	//template <typename T>
	CVImg CalculateSignedDistance(std::vector<AVector> contour);
	CVImg CalculateSignedDistance(std::vector<AVector> contour, CVImg aMask);
	float GetSignedDistance(std::vector<cv::Point> contour, int x, int y);

	// signed distance
	//template <typename T>
	CVImg CalculateSignedDistance(std::vector<std::vector<AVector>> contour);

	// Get contour
	std::vector<AVector> GetContour(CVImg img)
	{
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;
		findContours(img.GetCVImage(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		int longestIdx = GetLongestContourIndex(contours);
		return ConvertCVPointToAVector(contours[longestIdx]);
	}

	void SetWhite(std::string imageName)
	{
		cv::Mat img = _images[imageName];
		img = cv::Scalar(255, 255, 255);
	}

	// image
	void CreateImage(std::string imageName, int width, int height, int imgType);

	// image
	void ShowImage(std::string imageName);

	// image
	void SaveImage(std::string imageName, std::string filePath);

	// image
	void SetPixel(std::string imageName, int x, int y, MyColor col);

	// image
	int GetNumColumns(std::string imageName);

	// image
	int GetNumRows(std::string imageName);

	// convert (make this a template)
	std::vector<cv::Point2f>			ConvertAVectorToCvPoint2f(std::vector<AVector> contour);
	std::vector<cv::Point>				ConvertAVectorToCvPoint(std::vector<AVector> contour);
	std::vector<AVector>				ConvertCVPointToAVector(std::vector<cv::Point> contour);
	std::vector<AVector>				ConvertCVPointToAVector(std::vector<cv::Point2f> cvContour);
	std::vector<std::vector<cv::Point>> ConvertVectorsToCvPoint(std::vector<std::vector<AVector>> contours);

	// wait
	void WaitKey();

	// random color
	MyColor GetRandomColor();

	// drawing
	template <typename T>
	void DrawLine(std::string imageName, T pt1, T pt2, MyColor color, int thickness, float scale = 1.0f);

	// drawing
	void PutText(std::string imageName, std::string text, AVector pos, MyColor col, float scale = 0.5f, float thickness = 1)
	{
		cv::Mat img = _images[imageName];
		cv::putText(img, text, cv::Point(pos.x, pos.y), cv::FONT_HERSHEY_SIMPLEX, scale, cv::Scalar(col._b, col._g, col._r), thickness);
	}

	// drawing
	void DrawCircle(std::string imageName, AVector pt, MyColor col, float radius, float scale = 1.0f)
	{
		cv::Mat drawing = _images[imageName];
		cv::circle(drawing, cv::Point(pt.x * scale, pt.y * scale), radius, cv::Scalar(col._b, col._g, col._r), -1);
	}

	// get contour
	// findContours(indexedMats[a], contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	// drawing
	template <typename T>
	void DrawPolyOnCVImage(cv::Mat img,
		std::vector<T> shape_contours,
		MyColor color,
		bool isClosed,
		float thickness = 1.0f,
		float scale = 1.0f,
		float xOffset = 0,
		float yOffset = 0);

	// drawing
	template <typename T>
	void DrawPoly(std::string imageName,
		std::vector<T> shape_contours,
		MyColor color,
		bool isClosed,
		float thickness = 1.0f,
		float scale = 1.0f,
		float xOffset = 0,
		float yOffset = 0);

	// drawing
	template <typename T>
	void DrawRetranslatedPoly(std::string imageName,
		std::vector<T> shape_contours,
		std::vector<T> medial_axis,
		MyColor color,
		float thickness = 1.0f,
		float scale = 1.0f);

	// drawing
	template <typename T>
	void DrawFilledPoly(std::string imageName,
		std::vector<T> shape_contours,
		MyColor color,
		float scale = 1.0f,
		float xOffset = 0,
		float yOffset = 0);

	// drawing
	template <typename T>
	void DrawFilledPolyInt(CVImg& img,
		                   std::vector<T> shape_contours,
						   int val,
		                   float scale = 1.0f,
		                   float xOffset = 0,
		                   float yOffset = 0);

	// get longest contour
	template <typename T>
	int GetLongestContourIndex(std::vector<std::vector<T>> contours);

private:
	bool InsidePolygon(std::vector<AVector> boundary, AVector pt)
	{
		std::vector<cv::Point> cvBoundary = ConvertAVectorToCvPoint(boundary);
		return (cv::pointPolygonTest(cvBoundary, cv::Point(pt.x, pt.y), false) > 0);
	}
};

#endif
