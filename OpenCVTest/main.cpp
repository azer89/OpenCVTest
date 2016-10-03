// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OpenCVWrapper.h"

#include <algorithm>
#include <iostream>
#include <vector>

int _tmain(int argc, _TCHAR* argv[])
{
	// ---------- image thinning ----------
	/*OpenCVWrapper cvWrapper;
	CVImg bwImg;
	bwImg._name = "BW";
	bwImg.CreateGrayscaleImage(500, 500);
	bwImg.LoadGrayscale("C:/Users/azer/workspace/OpenCVTest/OpenCVTest/star.png");
	bwImg.Show();
	
	CVImg thinningImg = bwImg.Thinning();
	thinningImg._name = "Thinning";
	thinningImg.Show();	

	//for (int x = 0; x < 500; x++)
	//{
	//	for (int y = 0; y < 500; y++)
	//	{
	//		std::cout << thinningImg.GetGrayValue(x, y) << "\n";
	//	}
	//}
	
	cvWrapper.WaitKey();*/
	
	// ---------- wrapper ----------
	OpenCVWrapper cvWrapper;

	cvWrapper.CreateImage("hello_world", 500, 500, BGR_255);

	float slice = 7;
	float radius = 200;
	float pi_2 = CV_PI * 2.0f;

	std::vector<AVector> polyline;
	for (float a = 0; a < pi_2; a += (pi_2 / slice))
	{
		float x = 250 + sin(a) * radius;
		float y = 250 + cos(a) * radius;
		polyline.push_back(AVector(x, y));
	}

	//cvWrapper.DrawPoly("hello_world", polyline, MyColor(255), true, 3);
	//cvWrapper.ShowImage("hello_world");

	// ---------- demonstrating polygon filling with an integer image ----------	
	CVImg bwImg;
	bwImg.CreateIntegerImage(500);
	//cvWrapper.DrawFilledPolyInt(bwImg, polyline, 1);
	cvWrapper.DrawPolyInt(bwImg, polyline, 1, true, 3);

	for (int x = 0; x < 500; x++)
	{
		for (int y = 0; y < 500; y++)
		{
			if (bwImg.GetIntegerValue(x, y) != 0)
				{ cvWrapper.DrawCircle("hello_world", AVector(x, y), MyColor(255), 1); }
			else
				{ cvWrapper.DrawCircle("hello_world", AVector(x, y), MyColor(255, 0, 0), 1); }
		}
	}

	cvWrapper.ShowImage("hello_world");

	cvWrapper.WaitKey();

	/*std::vector<float> v{ 3, 1, -2, 1, -5.5, 9 };

	std::vector<float>::iterator result = std::min_element(std::begin(v), std::end(v));
	int idx = std::distance(std::begin(v), result);
	std::cout << "min element at: " << idx << "\n";
	std::cout << "the value is " << v[idx] << "\n";*/

	std::getchar();

	return 0;
}

