#include "pch.h"
#include "JarvisMarch.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

JarvisMarch::JarvisMarch()
{
}


JarvisMarch::~JarvisMarch()
{
}

std::vector<DirectX::SimpleMath::Vector2> JarvisMarch::GetConvexHull(const std::vector<DirectX::SimpleMath::Vector2>& points) const
{
	if (points.size() < 3)
	{
		std::cerr << "ERROR: 3 points are required at least!" << std::endl;
		exit(EXIT_FAILURE);
	}

	auto tmpPoints = points;
	//init result
	std::vector<Vector2> convexHull;

	//get first hull point and startpoint - use leftmost point
	Vector2 hullPoint = findLeftmost(tmpPoints);
	do
	{
		//add current found hull point
		convexHull.push_back(hullPoint);
		if (OnHullPointFoundEvent != nullptr)
			OnHullPointFoundEvent(convexHull);

		//initial endpoint for a candidate edge on the hull
		int endPointIndex = 0;
		for (int i = 1; i < tmpPoints.size(); ++i)
		{
			//iterate and check every point
			if (OnPointCheckEvent != nullptr)
				OnPointCheckEvent(tmpPoints[i]);
			float cross = crossProduct(hullPoint, tmpPoints[i], tmpPoints[endPointIndex]);
			if (tmpPoints[endPointIndex] == hullPoint || cross <= 0) //(cross <= 0) -> if current point is on the left side of the vector from current hullpoint to endpoint then use this as next hull point candidate 
			{
				//BUT if its on the vector from hullpoint to endpoint -> check if distance is bigger than last endpoint!
				if (cross == 0 && pointLenghtSquared(tmpPoints[i] - hullPoint) <= pointLenghtSquared(tmpPoints[endPointIndex] - hullPoint))
					continue;

				endPointIndex = i;
				if (OnHullCandidateFoundEvent != nullptr)
					OnHullCandidateFoundEvent(tmpPoints[endPointIndex]);
			}
		}
		//assign new leftmost point as new hull point
		hullPoint = tmpPoints[endPointIndex];
		//erase found convex hull point from list
		tmpPoints.erase(tmpPoints.begin() + endPointIndex);
	}
	//do until we wrapped around to the first hull point
	while (hullPoint != convexHull[0]);

	if (OnHullCompleteEvent != nullptr)
		OnHullCompleteEvent(convexHull);

	return convexHull;
}

DirectX::SimpleMath::Vector2 JarvisMarch::findLeftmost(const std::vector<DirectX::SimpleMath::Vector2>& dataArray)
{
	int leftmostIndex = 0;
	for (int i = 1; i < dataArray.size(); ++i)
	{
		//check for the smallest x and use this index for leftmost point
		if (dataArray[i].x < dataArray[leftmostIndex].x)
			leftmostIndex = i;
	}
	return dataArray[leftmostIndex];
}

bool JarvisMarch::isOnTheLeftSideOfLine(const DirectX::SimpleMath::Vector2& from, const DirectX::SimpleMath::Vector2& point, const DirectX::SimpleMath::Vector2& to)
{
	//return cross product (if result < 0 its on the left side of the vector (to-from) and if result > 0 its on the right side)
	return crossProduct(from, point, to) < 0;
}

float JarvisMarch::crossProduct(const DirectX::SimpleMath::Vector2& from, const DirectX::SimpleMath::Vector2& point, const DirectX::SimpleMath::Vector2& to)
{
	float x1 = to.x - from.x;
	float y1 = to.y - from.y;
	float x2 = point.x - from.x;
	float y2 = point.y - from.y;

	return x1*y2 - y1*x2;
}

float JarvisMarch::pointLenghtSquared(const DirectX::SimpleMath::Vector2& point)
{
	return point.x * point.x + point.y * point.y;
}
