// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OpenCVWrapper.h"

#include <iostream>
#include <vector>

int _tmain(int argc, _TCHAR* argv[])
{
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

	cvWrapper.DrawPoly("hello_world", polyline, MyColor(255), true, 3);
	cvWrapper.ShowImage("hello_world");
	cvWrapper.WaitKey();

	return 0;
}

