#include "pch.h"
#include "CollisionVisualizer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

CollisionVisualizer::CollisionVisualizer()
{
}


CollisionVisualizer::~CollisionVisualizer()
{
}

std::vector<Vector2> CollisionVisualizer::CreateCircleVerticesLineStrip(const float& radius)
{
	std::vector<Vector2> vertices;
	createCircleVertices(vertices, radius, 100);
	vertices.push_back(vertices[0]);
	return vertices;
}

std::vector<Vector2> CollisionVisualizer::CreateCircleVerticesTriangleFan(const DirectX::SimpleMath::Vector2& center, const float& radius)
{
	std::vector<Vector2> vertices;
	vertices.push_back(center);
	createCircleVertices(vertices, radius, 100);
	return vertices;
}

void CollisionVisualizer::createCircleVertices(std::vector<DirectX::SimpleMath::Vector2>& vertices, const float& radius, const float& resolution)
{
	for (float i = 0; i <= resolution; i += 1.0f)
	{
		float t = XM_PI * 2 * i / resolution;
		vertices.push_back(Vector2(radius * cos(t), radius * -sin(t)));
	}
}
