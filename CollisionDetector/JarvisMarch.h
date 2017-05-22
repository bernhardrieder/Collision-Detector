#pragma once

class JarvisMarch
{
public:
	JarvisMarch();
	~JarvisMarch();

	std::vector<DirectX::SimpleMath::Vector2> GetConvexHull(const std::vector<DirectX::SimpleMath::Vector2>& points) const;

	std::function<void(const DirectX::SimpleMath::Vector2&)> OnPointCheckEvent = nullptr;
	std::function<void(const DirectX::SimpleMath::Vector2&)> OnHullCandidateFoundEvent = nullptr;
	std::function<void(const std::vector<DirectX::SimpleMath::Vector2>&)> OnHullPointFoundEvent = nullptr;
	std::function<void(const std::vector<DirectX::SimpleMath::Vector2>&)> OnHullCompleteEvent = nullptr;

private:
	static DirectX::SimpleMath::Vector2 findLeftmost(const std::vector<DirectX::SimpleMath::Vector2>& DataArray);
	static bool isOnTheLeftSideOfLine(const DirectX::SimpleMath::Vector2& from, const DirectX::SimpleMath::Vector2& point, const DirectX::SimpleMath::Vector2& to);
	static float crossProduct(const DirectX::SimpleMath::Vector2& from, const DirectX::SimpleMath::Vector2& point, const DirectX::SimpleMath::Vector2& to);
	static float pointLenghtSquared(const DirectX::SimpleMath::Vector2& point);
};