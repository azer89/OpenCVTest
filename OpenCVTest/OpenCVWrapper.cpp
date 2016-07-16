
#include "OpenCVWrapper.h"
//#include "AVector.h"
//#include "ARectangle.h"
//#include "UtilityFunctions.h"

/*
================================================================================
================================================================================
*/
OpenCVWrapper::OpenCVWrapper()
{
	this->_rng = cv::RNG(12345);
}

/*
================================================================================
================================================================================
*/
OpenCVWrapper::~OpenCVWrapper()
{
}

/*
================================================================================
================================================================================
*/
MyColor OpenCVWrapper::GetRandomColor()
{
	return MyColor(_rng.uniform(0, 255), _rng.uniform(0, 255), _rng.uniform(0, 255));
}

/*
================================================================================
================================================================================
*/
void OpenCVWrapper::CreateImage(std::string imageName, int width, int height, int imgType)
{
	cv::Mat newImg = cv::Mat::zeros(width, height, imgType);
	_images[imageName] = newImg;
}

/*
================================================================================
================================================================================
*/
void OpenCVWrapper::ShowImage(std::string imageName)
{
	cv::Mat img = _images[imageName];
	cv::namedWindow(imageName, CV_WINDOW_AUTOSIZE);
	cv::imshow(imageName, img);
}

/*
================================================================================
================================================================================
*/
void OpenCVWrapper::SaveImage(std::string imageName, std::string filePath)
{
	cv::Mat img = _images[imageName];
	cv::imwrite(filePath, img);
}

/*
================================================================================
================================================================================
*/
// BGR_255
void OpenCVWrapper::SetPixel(std::string imageName, int x, int y, MyColor col)
{
	cv::Mat img = _images[imageName];
	img.at<cv::Vec3b>(y, x)[0] = col._r;
	img.at<cv::Vec3b>(y, x)[1] = col._g;
	img.at<cv::Vec3b>(y, x)[2] = col._b;
}

/*
================================================================================
================================================================================
*/
int OpenCVWrapper::GetNumColumns(std::string imageName)
{
	cv::Mat img = _images[imageName];
	return img.cols;
}

/*
================================================================================
================================================================================
*/
int OpenCVWrapper::GetNumRows(std::string imageName)
{
	cv::Mat img = _images[imageName];
	return img.rows;
}

/*
================================================================================
================================================================================
*/
void OpenCVWrapper::WaitKey()
{
	cv::waitKey();
}

/*
================================================================================
================================================================================
*/
std::vector<cv::Point2f> OpenCVWrapper::ConvertAVectorToCvPoint2f(std::vector<AVector> contour)
{
	std::vector<cv::Point2f> cvContour;
	for (int a = 0; a < contour.size(); a++)
	{
		cvContour.push_back(cv::Point2f(contour[a].x, contour[a].y));
	}
	return cvContour;
}

/*
================================================================================
================================================================================
*/
template <typename T>
float OpenCVWrapper::PointPolygonTest(std::vector<T> shape_contours, T pt)
{
	std::vector<cv::Point2f> new_contours;
	for (size_t a = 0; a < shape_contours.size(); a++)
	{
		new_contours.push_back(cv::Point2f((shape_contours[a].x), (shape_contours[a].y)));
	}

	return cv::pointPolygonTest(new_contours, cv::Point2f(pt.x, pt.y), true);
}

template
float OpenCVWrapper::PointPolygonTest(std::vector<AVector> shape_contours, AVector pt);


/*
================================================================================
================================================================================
*/
template <typename T>
void OpenCVWrapper::DrawPolyOnCVImage(cv::Mat img,
	std::vector<T> shape_contours,
	MyColor color,
	bool isClosed,
	float thickness,
	float scale,
	float xOffset,
	float yOffset)
{
	std::vector<cv::Point2f> new_contours;
	for (size_t b = 0; b < shape_contours.size(); b++)
	{
		new_contours.push_back(cv::Point2f((shape_contours[b].x * scale + xOffset), (shape_contours[b].y * scale + yOffset)));
	}
	for (size_t b = 0; b < new_contours.size() - 1; b++)
	{
		cv::line(img, new_contours[b], new_contours[b + 1], cv::Scalar(color._b, color._g, color._r), thickness);
	}
	if (isClosed)
	{
		cv::line(img, new_contours[new_contours.size() - 1], new_contours[0], cv::Scalar(color._b, color._g, color._r), thickness);
	}
}

template
void OpenCVWrapper::DrawPolyOnCVImage(cv::Mat img,
std::vector<AVector> shape_contours,
MyColor color,
bool isClosed,
float thickness,
float scale,
float xOffset,
float yOffset);

template
void OpenCVWrapper::DrawPolyOnCVImage(cv::Mat img,
std::vector<cv::Point2f> shape_contours,
MyColor color,
bool isClosed,
float thickness,
float scale,
float xOffset,
float yOffset);


/*
================================================================================
================================================================================
*/
template <typename T>
void OpenCVWrapper::DrawPoly(std::string imageName,
	std::vector<T> shape_contours,
	MyColor color,
	bool isClosed,
	float thickness,
	float scale,
	float xOffset,
	float yOffset)
{
	cv::Mat drawing = _images[imageName];

	std::vector<cv::Point2f> new_contours;
	for (size_t b = 0; b < shape_contours.size(); b++)
	{
		new_contours.push_back(cv::Point2f((shape_contours[b].x * scale + xOffset), (shape_contours[b].y * scale + yOffset)));
	}
	for (size_t b = 0; b < new_contours.size() - 1; b++)
	{
		cv::line(drawing, new_contours[b], new_contours[b + 1], cv::Scalar(color._b, color._g, color._r), thickness);
	}
	if (isClosed)
	{
		cv::line(drawing, new_contours[new_contours.size() - 1], new_contours[0], cv::Scalar(color._b, color._g, color._r), thickness);
	}
}

template
void OpenCVWrapper::DrawPoly(std::string imageName,
std::vector<AVector> shape_contours,
MyColor color,
bool isClosed,
float thickness,
float scale,
float xOffset,
float yOffset);

/*
================================================================================
================================================================================
*/
template <typename T>
void OpenCVWrapper::DrawRetranslatedPoly(std::string imageName,
	std::vector<T> shape_contours,
	std::vector<T> medial_axis,
	MyColor color,
	float thickness,
	float scale)
{
	std::cerr << "not implemented\n";

	/*
	cv::Mat drawing = _images[imageName];

	ARectangle bb = UtilityFunctions::GetBoundingBox(shape_contours);

	std::vector<cv::Point2f> new_contours1;
	std::vector<cv::Point2f> new_contours2;
	for (size_t b = 0; b < shape_contours.size(); b++)
		{ new_contours1.push_back(cv::Point2f((shape_contours[b].x - bb.topleft.x) * scale, (shape_contours[b].y - bb.topleft.y) * scale)); }
	for (size_t b = 0; b < medial_axis.size(); b++)
		{ new_contours2.push_back(cv::Point2f((medial_axis[b].x - bb.topleft.x) * scale, (medial_axis[b].y - bb.topleft.y) * scale)); }

	for (size_t b = 0; b < new_contours1.size() - 1; b++)
		{ cv::line(drawing, new_contours1[b], new_contours1[b + 1], cv::Scalar(color._b, color._g, color._r), thickness); }
	cv::line(drawing, new_contours1[new_contours1.size() - 1], new_contours1[0], cv::Scalar(color._b, color._g, color._r), thickness);

	for (size_t b = 0; b < new_contours2.size() - 1; b++)
		{ cv::line(drawing, new_contours2[b], new_contours2[b + 1], cv::Scalar(color._b, color._g, color._r), thickness + 2); }

	*/
}

template
void OpenCVWrapper::DrawRetranslatedPoly(std::string imageName,
std::vector<AVector> shape_contours,
std::vector<AVector> medial_axis,
MyColor color,
float thickness,
float scale);



/*
================================================================================
================================================================================
*/
template <typename T>
void OpenCVWrapper::DrawFilledPoly(std::string imageName,
	std::vector<T> shape_contours,
	MyColor color,
	float scale,
	float xOffset,
	float yOffset)
{
	cv::Mat drawing = _images[imageName];

	std::vector<cv::Point> new_contours;
	for (size_t b = 0; b < shape_contours.size() - 1; b++)
	{
		new_contours.push_back(cv::Point((int)(shape_contours[b].x * scale + xOffset), int(shape_contours[b].y * scale + yOffset)));
	}
	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(new_contours);
	cv::fillPoly(drawing, contours, cv::Scalar(color._b, color._g, color._r));
}

template
void OpenCVWrapper::DrawFilledPoly(std::string imageName,
std::vector<AVector> shape_contours,
MyColor color,
float scale,
float xOffset,
float yOffset);

/*
================================================================================
================================================================================
*/
template <typename T>
void OpenCVWrapper::DrawLine(std::string imageName, T pt1, T pt2, MyColor color, int thickness, float scale)
{
	cv::Mat drawing = _images[imageName];
	cv::line(drawing, cv::Point(pt1.x, pt1.y) * scale, cv::Point(pt2.x, pt2.y) * scale, cv::Scalar(color._b, color._g, color._r), thickness);
}

template
void OpenCVWrapper::DrawLine(std::string imageName, AVector pt1, AVector pt2, MyColor color, int thickness, float scale);