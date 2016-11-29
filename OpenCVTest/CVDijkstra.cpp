
// http_://codereview.stackexchange.com/questions/66724/more-optimized-approach-of-dijkstras-algorithm

#include "CVDijkstra.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include <queue>
#include <algorithm>

#include "OpenCVWrapper.h"

// dijkstra
//#define UNKNOWN_CELL 197
#define FREE_CELL 255
#define OCCUPIED_CELL 0
//#define map(x,y) image.data[x*image.cols+y]
int get_cell_at(const cv::Mat & image, int x, int y)
{
	assert(x < image.rows);
	assert(y < image.cols);
	return image.data[x * image.cols + y];
}

// dijkstra
typedef struct vertex{
	cv::Point2i id_;
	cv::Point2i from_;

	vertex(cv::Point2i id, cv::Point2i from)
	{
		id_ = id;
		from_ = from;
	}

} vertex;

// dijkstra
struct CompareID
{
	CompareID(cv::Point2i val) : val_(val) {}
	bool operator()(std::pair<double, vertex> const elem) const {
		return val_ == elem.second.id_;
	}
private:
	cv::Point2i val_;
};

// dijkstra
bool checkIfNotOutOfBounds(cv::Point2i current, int rows, int cols)
{
	return (current.x >= 0 && current.y >= 0 &&
		current.x < cols && current.y < rows);
}

void CVDijkstra::DijkstraTest()
{
	int dim = 100;

	cv::Mat image = cv::Mat(dim, dim, CV_8UC1);

	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
		{
			image.data[i*image.cols + j] = FREE_CELL;

			if (j == image.cols / 2 && (i > 3 && i < image.rows - 3))
				image.data[i*image.cols + j] = OCCUPIED_CELL;
		}
	// Create working and visited set.
	std::multimap<double, vertex> working, visited;

	// Goal top right
	cv::Point2i goal(image.cols - 5, image.rows - 5);

	//std::cout << "goal : " << goal.x << ", " << goal.y << "\n";
	// Start bottom left
	cv::Point2i start(0, image.rows - 1);
	//cv::Point2i start(10, 5);

	// Time the algorithm.
	//Timer timer;
	//timer.start();
	// Initialize working set
	working.insert(std::make_pair(0, vertex(goal, goal)));

	// Conditions in continuing
	// 1.) Working is empty implies all nodes are visited.
	// 2.) If the start is still not found in the working visited set.
	// The Dijkstra's algorithm
	while (!working.empty() && std::find_if(visited.begin(), visited.end(), CompareID(start)) == visited.end())
	{

		// Get the top of the STL.
		// It is already given that the top of the multimap has the lowest cost.
		std::pair<double, vertex> currentPair = *working.begin();
		cv::Point2i current = currentPair.second.id_;
		visited.insert(currentPair);
		working.erase(working.begin());

		// Check all arcs
		// Only insert the cells into working under these 3 conditions:
		// 1. The cell is not in visited cell
		// 2. The cell is not out of bounds
		// 3. The cell is free
		for (int x = current.x - 1; x <= current.x + 1; x++)
			for (int y = current.y - 1; y <= current.y + 1; y++)
			{

				if (std::find_if(visited.begin(), visited.end(), CompareID(cv::Point2i(x, y))) == visited.end() &&
					checkIfNotOutOfBounds(cv::Point2i(x, y), image.rows, image.cols) &&
					get_cell_at(image, x, y) == FREE_CELL)
				{
					vertex newVertex = vertex(cv::Point2i(x, y), current);

					// ===== COST =====
					double cost = currentPair.first + sqrt(2);
					// Cost is 1
					if (x == current.x || y == current.y)
						cost = currentPair.first + 1;

					std::multimap<double, vertex>::iterator it =
						std::find_if(working.begin(), working.end(), CompareID(cv::Point2i(x, y)));
					if (it == working.end())
						working.insert(std::make_pair(cost, newVertex));
					else if (cost < (*it).first)
					{
						std::multimap<double, vertex>::iterator itCopy = it;
						++it;
						working.erase(itCopy);
						working.insert(std::make_pair(cost, newVertex));
					}
				}
			}

		//std::multimap<double, vertex>::iterator it = working.begin();
	}
	//std::cerr << "Time elapsed: " << timer.getElapsedTimeInMilliSec() << " ms";

	// Recover Path
	cv::cvtColor(image, image, CV_GRAY2BGRA);
	int cn = image.channels();
	if (std::find_if(visited.begin(), visited.end(), CompareID(start)) != visited.end())
	{
		std::cerr << "Path found!" << std::endl;
		std::pair <double, vertex> currentPair = *std::find_if(visited.begin(), visited.end(), CompareID(start));

		std::cerr << "Point(x,y) = " << currentPair.second.id_.x << "," << currentPair.second.id_.y << std::endl;
		do
		{
			image.data[currentPair.second.id_.x*cn*image.cols + currentPair.second.id_.y*cn + 0] = 0;
			image.data[currentPair.second.id_.x*cn*image.cols + currentPair.second.id_.y*cn + 1] = 255;
			image.data[currentPair.second.id_.x*cn*image.cols + currentPair.second.id_.y*cn + 2] = 0;
			
			/*int x = currentPair.second.id_.x;
			int y = currentPair.second.id_.y;
			image.at<cv::Vec3b>(y, x)[0] = 0; // blue
			image.at<cv::Vec3b>(y, x)[1] = 255; // green
			image.at<cv::Vec3b>(y, x)[2] = 0; // red*/
			
			currentPair = *std::find_if(visited.begin(), visited.end(), CompareID(currentPair.second.from_));
			std::cerr << "Point(x,y) = " << currentPair.second.id_.x << "," << currentPair.second.id_.y << std::endl;
		} while (currentPair.second.id_.x != goal.x || currentPair.second.id_.y != goal.y);

	}
	else
		std::cerr << "Path not found!" << std::endl;

	// start red
	image.data[start.x*cn*image.cols + start.y*cn + 0] = 0;
	image.data[start.x*cn*image.cols + start.y*cn + 1] = 0;
	image.data[start.x*cn*image.cols + start.y*cn + 2] = 255;

	// goal blue
	image.data[goal.x*cn*image.cols + goal.y*cn + 0] = 255;
	image.data[goal.x*cn*image.cols + goal.y*cn + 1] = 0;
	image.data[goal.x*cn*image.cols + goal.y*cn + 2] = 0;

	cv::imshow("Map with path", image);
	cv::waitKey();
}