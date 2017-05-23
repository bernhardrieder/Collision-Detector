#pragma once
class CollisionVisualizer
{
public:
	CollisionVisualizer();
	~CollisionVisualizer();

	static std::vector<DirectX::SimpleMath::Vector2> CreateCircleVerticesLineStrip(const float& radius);
	static std::vector<DirectX::SimpleMath::Vector2> CreateCircleVerticesTriangleFan(const DirectX::SimpleMath::Vector2& center, const float& radius);

private:
	static void createCircleVertices(std::vector<DirectX::SimpleMath::Vector2>& vertices, const float& radius, const float& resolution);
};

