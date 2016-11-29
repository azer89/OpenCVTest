
#include "OpenCVWrapper.h"
#include "AVector.h"
//#include "ARectangle.h"
//#include "UtilityFunctions.h"

#include "SystemParams.h"

/*================================================================================
================================================================================*/
OpenCVWrapper::OpenCVWrapper()
{
	this->_rng = cv::RNG(12345);
}

/*================================================================================
================================================================================*/
OpenCVWrapper::~OpenCVWrapper()
{
}

/*================================================================================
================================================================================*/
MyColor OpenCVWrapper::GetRandomColor()
{	
	return MyColor(_rng.uniform(0, 255), _rng.uniform(0, 255), _rng.uniform(0, 255));
}

/*================================================================================
================================================================================*/
void OpenCVWrapper::CreateImage(std::string imageName, int width, int height, int imgType)
{
	cv::Mat newImg = cv::Mat::zeros(width, height, imgType);
	_images[imageName] = newImg;
}

/*================================================================================
================================================================================*/
void OpenCVWrapper::ShowImage(std::string imageName)
{
	cv::Mat img = _images[imageName];
	cv::namedWindow(imageName, CV_WINDOW_AUTOSIZE);
	cv::imshow(imageName, img);
}


/*================================================================================
================================================================================*/
void OpenCVWrapper::SaveImage(std::string imageName, std::string filePath)
{
	cv::Mat img = _images[imageName];
	cv::imwrite(filePath, img);
}

/*================================================================================
BGR_255
================================================================================*/
MyColor OpenCVWrapper::GetColor(std::string imageName, int x, int y)
{
	// flipped?
	cv::Mat img = _images[imageName];
	int r = img.at<cv::Vec3b>(y, x)[0];
	int g = img.at<cv::Vec3b>(y, x)[1];
	int b = img.at<cv::Vec3b>(y, x)[2];
	return MyColor(r, g, b);
}

/*================================================================================
================================================================================*/
std::vector<AVector> OpenCVWrapper::GetContour(CVImg img)
{
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(img.GetCVImage(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	int longestIdx = GetLongestContourIndex(contours);
	return ConvertList<cv::Point, AVector>(contours[longestIdx]);
}

/*================================================================================
================================================================================*/
std::vector<std::vector<AVector>> OpenCVWrapper::GetContours(CVImg img)
{
	std::vector<std::vector<AVector>> myContours;
	std::vector<std::vector<cv::Point> > cvContours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(img.GetCVImage(), cvContours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	for (int a = 0; a < cvContours.size(); a++)
	{
		std::vector<AVector> ctr = ConvertList<cv::Point, AVector>(cvContours[a]);
		myContours.push_back(ctr);
	}

	return myContours;
}


/*================================================================================
BGR_255
================================================================================*/
void OpenCVWrapper::SetPixel(std::string imageName, int x, int y, MyColor col)
{
	// flipped?
	cv::Mat img = _images[imageName];
	img.at<cv::Vec3b>(y, x)[0] = col._b;
	img.at<cv::Vec3b>(y, x)[1] = col._g;
	img.at<cv::Vec3b>(y, x)[2] = col._r;
}

/*================================================================================
================================================================================*/
int OpenCVWrapper::GetNumColumns(std::string imageName)
{
	cv::Mat img = _images[imageName];
	return img.cols;
}

/*================================================================================
================================================================================*/
int OpenCVWrapper::GetNumRows(std::string imageName)
{
	cv::Mat img = _images[imageName];
	return img.rows;
}

/*================================================================================
================================================================================*/
void OpenCVWrapper::WaitKey()
{
	cv::waitKey();
}

/*================================================================================
================================================================================*/
void OpenCVWrapper::SetWhite(std::string imageName)
{
	cv::Mat img = _images[imageName];
	img = cv::Scalar(255, 255, 255);
}

/*================================================================================
================================================================================*/
void OpenCVWrapper::SetBlack(std::string imageName)
{
	cv::Mat img = _images[imageName];
	img = cv::Scalar(0, 0, 0);
}

/*================================================================================
================================================================================*/
CVImg OpenCVWrapper::ConvertToBW(std::string imageName)
{
	//std::cout << "ConvertToBW\n";
	float sz = SystemParams::_upscaleFactor;

	cv::Mat img = _images[imageName];
	CVImg bwImg;
	//std::cout << img.rows << " " << img.cols << "\n";
	bwImg.CreateIntegerImage(sz, sz);

	for (int x = 0; x < sz; x++)
	{
		for (int y = 0; y < sz; y++)
		{
			if (img.at<cv::Vec3b>(y, x)[0] == 0 && img.at<cv::Vec3b>(y, x)[1] == 0 && img.at<cv::Vec3b>(y, x)[2] == 0)
			{
				bwImg.SetIntegerPixel(x, y, 0);
			}
			else
			{
				bwImg.SetIntegerPixel(x, y, 1);
			}
		}
	}
	return bwImg;
}

/*================================================================================
================================================================================*/
/*std::vector<cv::Point2f> OpenCVWrapper::ConvertAVectorToCvPoint2f(std::vector<AVector> contour)
{
	std::vector<cv::Point2f> cvContour;
	for (int a = 0; a < contour.size(); a++)
		{ cvContour.push_back(cv::Point2f(contour[a].x, contour[a].y)); }
	return cvContour;
}*/

/*================================================================================
================================================================================*/
template <typename T, typename U>
std::vector<U> OpenCVWrapper::ConvertList(std::vector<T> contour1)
{
	std::vector<U> contour2;
	for (int a = 0; a < contour1.size(); a++)
	{
		contour2.push_back(U(contour1[a].x, contour1[a].y));
	}
	return contour2;
}

// Point   --> AVector
template std::vector<AVector>     OpenCVWrapper::ConvertList(std::vector<cv::Point> contour1);

// AVector --> Point
template std::vector<cv::Point>   OpenCVWrapper::ConvertList(std::vector<AVector> contour1);

// Point2f --> AVector
template std::vector<AVector>     OpenCVWrapper::ConvertList(std::vector<cv::Point2f> contour1);

// AVector --> Point2f
template std::vector<cv::Point2f> OpenCVWrapper::ConvertList(std::vector<AVector> contour1);

/*================================================================================
================================================================================*/
/*std::vector<cv::Point> OpenCVWrapper::ConvertAVectorToCvPoint(std::vector<AVector> contour)
{
	std::vector<cv::Point> cvContour;
	for (int a = 0; a < contour.size(); a++)
		{ cvContour.push_back(cv::Point(contour[a].x, contour[a].y)); }
	return cvContour;
}*/

/*================================================================================
================================================================================*/
template <typename T>
float OpenCVWrapper::PointPolygonTest(std::vector<T> shape_contours, T pt)
{
	std::vector<cv::Point2f> new_contours;
	for (size_t a = 0; a < shape_contours.size(); a++)
		{ new_contours.push_back(cv::Point2f((shape_contours[a].x), (shape_contours[a].y))); }

	return cv::pointPolygonTest(new_contours, cv::Point2f(pt.x, pt.y), true);
}

template
float OpenCVWrapper::PointPolygonTest(std::vector<AVector> shape_contours, AVector pt);

/*================================================================================
================================================================================*/
CVImg OpenCVWrapper::CalculateSignedDistance(std::vector<AVector> contour)
{
	//std::vector<cv::Point> cvContour = ConvertAVectorToCvPoint(contour);
	std::vector<cv::Point> cvContour = ConvertList<AVector, cv::Point>(contour);

	int sz = SystemParams::_upscaleFactor;

	CVImg rawDist;
	rawDist.CreateFloatImage(sz);

	for (int x = 0; x < sz; x++)
	{
		for (int y = 0; y < sz; y++)
		{
			float d = cv::pointPolygonTest(cvContour, cv::Point2f(x, y), true);
			rawDist.SetFloatPixel(x, y, -d); // because I want positive value outside
		}
	}

	return rawDist;
}

/*================================================================================
================================================================================*/
CVImg OpenCVWrapper::CalculateSignedDistance(std::vector<AVector> contour, CVImg aMask)
{
	//std::vector<cv::Point> cvContour = ConvertAVectorToCvPoint(contour);
	std::vector<cv::Point> cvContour = ConvertList<AVector, cv::Point>(contour);

	int sz = SystemParams::_upscaleFactor;

	CVImg rawDist;
	rawDist.CreateFloatImage(sz);

	for (int x = 0; x < sz; x++)
	{
		for (int y = 0; y < sz; y++)
		{
			float d = sz * sz;
			//if (UtilityFunctions::InsidePolygon(boundary, AVector(x, y)))
			if (aMask.GetIntegerValue(x, y) > 0)
			{
				// because I want positive value outside
				d = -cv::pointPolygonTest(cvContour, cv::Point2f(x, y), true);
			}
			rawDist.SetFloatPixel(x, y, d);
		}
	}
	return rawDist;
}

/*================================================================================
================================================================================*/
float OpenCVWrapper::GetSignedDistance(std::vector<cv::Point> contour, int x, int y)
{
	// https_:_//github.com/opencv/opencv/blob/master/modules/imgproc/src/geometry.cpp
	return -cv::pointPolygonTest(contour, cv::Point2f(x, y), true);
}

/*================================================================================
================================================================================*/
float OpenCVWrapper::GetSignedDistance(std::vector<std::vector<cv::Point>> contours, int x, int y)
{
	//return -cv::pointPolygonTest(contour, cv::Point2f(x, y), true);
	float dist = std::numeric_limits<float>::max();
	for (int a = 0; a < contours.size(); a++)
	{
		float d = GetSignedDistance(contours[a], x, y);
		if (d < dist) { dist = d; }
	}
	return dist;
}

/*================================================================================
================================================================================*/
std::vector<AVector> OpenCVWrapper::GetConvexHull(std::vector<AVector> polygon)
{
	std::vector<cv::Point2f> cvPolygon = ConvertList<AVector, cv::Point2f>(polygon);
	std::vector<cv::Point2f> cvHull;
	cv::convexHull(cv::Mat(cvPolygon), cvHull, false);

	return ConvertList<cv::Point2f, AVector>(cvHull);
}

/*================================================================================
================================================================================*/
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
		{ new_contours.push_back(cv::Point2f((shape_contours[b].x * scale + xOffset), (shape_contours[b].y * scale + yOffset))); }
	for (size_t b = 0; b < new_contours.size() - 1; b++)
		{ cv::line(img, new_contours[b], new_contours[b + 1], cv::Scalar(color._b, color._g, color._r) , thickness); }
	if (isClosed)
		{ cv::line(img, new_contours[new_contours.size() - 1], new_contours[0], cv::Scalar(color._b, color._g, color._r), thickness); }
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


/*================================================================================
================================================================================*/
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
		{ new_contours.push_back(cv::Point2f((shape_contours[b].x * scale + xOffset), (shape_contours[b].y * scale + yOffset))); }
	for (size_t b = 0; b < new_contours.size() - 1; b++)
		{ cv::line(drawing, new_contours[b], new_contours[b + 1], cv::Scalar(color._b, color._g, color._r) , thickness); }
	if (isClosed)
		{ cv::line(drawing, new_contours[new_contours.size() - 1], new_contours[0], cv::Scalar(color._b, color._g, color._r), thickness); }
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

/*================================================================================
================================================================================*/
template <typename T>
void OpenCVWrapper::DrawPolys(std::string imageName,
	                          std::vector<std::vector<T>> shape_contours,
	                          MyColor color,
	                          bool isClosed,
	                          float thickness,
	                          float scale,
	                          float xOffset,
	                          float yOffset)
{
	for (int a = 0; a < shape_contours.size(); a++)
	{
		DrawPoly(imageName,
			     shape_contours[a],
			     color,
			     isClosed,
			     thickness,
			     scale,
			     xOffset,
			     yOffset);
	}
}

template
void OpenCVWrapper::DrawPolys(std::string imageName,
	                          std::vector<std::vector<AVector>> shape_contours,
	                          MyColor color,
	                          bool isClosed,
	                          float thickness,
	                          float scale,
	                          float xOffset,
	                          float yOffset);

/*template <typename T>
void OpenCVWrapper::DrawRetranslatedPoly(std::string imageName,
							             std::vector<T> shape_contours, 
							             std::vector<T> medial_axis, 
										 MyColor color,
										 float thickness,
										 float scale)
{
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
}

template
void OpenCVWrapper::DrawRetranslatedPoly(std::string imageName,
							             std::vector<AVector> shape_contours, 
										 std::vector<AVector> medial_axis,
										 MyColor color,
										 float thickness,
										 float scale );*/



/*================================================================================
================================================================================*/
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
	for (size_t b = 0; b < shape_contours.size(); b++)
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

/*================================================================================
================================================================================*/
template <typename T>
void OpenCVWrapper::DrawFilledPolyInt(CVImg& img,
	                                  std::vector<T> shape_contours,
									  int val,
	                                  float scale,
	                                  float xOffset,
	                                  float yOffset)
{
	std::vector<cv::Point> new_contours;
	for (size_t b = 0; b < shape_contours.size() - 1; b++)
		{ new_contours.push_back(cv::Point((int)(shape_contours[b].x * scale + xOffset), int(shape_contours[b].y * scale + yOffset))); }
	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(new_contours);
	cv::fillPoly(img._img, contours, val);
}

template
void OpenCVWrapper::DrawFilledPolyInt(CVImg& img,
	                                  std::vector<AVector> shape_contours,
									  int val,
	                                  float scale,
	                                  float xOffset,
	                                  float yOffset);

/*================================================================================
================================================================================*/
template <typename T>
void OpenCVWrapper::DrawPolyInt(CVImg& img,
	                            std::vector<T> shape_contours,
	                            int val,
	                            bool isClosed,
	                            float thickness,
	                            float scale,
	                            float xOffset,
	                            float yOffset)
{
	std::vector<cv::Point2f> new_contours;
	for (size_t b = 0; b < shape_contours.size(); b++)
		{ new_contours.push_back(cv::Point2f((shape_contours[b].x * scale + xOffset), (shape_contours[b].y * scale + yOffset))); }
	for (size_t b = 0; b < new_contours.size() - 1; b++)
		{ cv::line(img._img, new_contours[b], new_contours[b + 1], val , thickness); }
	if (isClosed)
		{ cv::line(img._img, new_contours[new_contours.size() - 1], new_contours[0], val, thickness); }
}

template
void OpenCVWrapper::DrawPolyInt(CVImg& img,
	                            std::vector<AVector> shape_contours,
								int val,
	                            bool isClosed,
	                            float thickness,
	                            float scale,
	                            float xOffset,
	                            float yOffse);

/*================================================================================
================================================================================*/
template <typename T>
void OpenCVWrapper::DrawLineInt(CVImg& img, T pt1, T pt2, int val, int thickness, float scale)
{
	cv::line(img._img, cv::Point(pt1.x, pt1.y) * scale, cv::Point(pt2.x, pt2.y) * scale, val, thickness);
}

template
void OpenCVWrapper::DrawLineInt(CVImg& img, AVector pt1, AVector pt2, int val, int thickness, float scale);

/*================================================================================
================================================================================*/
template <typename T>
void OpenCVWrapper::DrawLine(std::string imageName, T pt1, T pt2, MyColor color, int thickness, float scale)
{
	cv::Mat drawing = _images[imageName];
	cv::line(drawing, cv::Point(pt1.x, pt1.y) * scale, cv::Point(pt2.x, pt2.y) * scale, cv::Scalar(color._b, color._g, color._r), thickness);
}

template
void OpenCVWrapper::DrawLine(std::string imageName, AVector pt1, AVector pt2, MyColor color, int thickness, float scale);

/*================================================================================
================================================================================*/
template <typename T>
int OpenCVWrapper::GetLongestContourIndex(std::vector<std::vector<T>> contours)
{
	if (contours.size() == 1) { return 0; }
	else if (contours.size() == 0) { std::cout << "GetLongestContour function says: error no contour\n"; }

	int idx = -1;
	int sz = -1;
	for (int a = 0; a < contours.size(); a++)
	{
		int s = contours[a].size();
		if (s > sz)
		{
			sz = s;
			idx = a;
		}
	}
	return idx;
}

template int OpenCVWrapper::GetLongestContourIndex<cv::Point>(std::vector<std::vector<cv::Point>> contours); // opencv
template int OpenCVWrapper::GetLongestContourIndex<AVector>(std::vector<std::vector<AVector>> contours); // AVector

/*================================================================================
================================================================================*/
void OpenCVWrapper::PutText(std::string imageName, std::string text, AVector pos, MyColor col, float scale, float thickness)
{
	cv::Mat img = _images[imageName];
	cv::putText(img, text, cv::Point(pos.x, pos.y), cv::FONT_HERSHEY_SIMPLEX, scale, cv::Scalar(col._b, col._g, col._r), thickness);
}