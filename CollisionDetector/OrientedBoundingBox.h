#pragma once
namespace CollisionDetection
{
	class OrientedBoundingBox
	{
	public:
		OrientedBoundingBox();
		~OrientedBoundingBox();

		void CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points);
		void Transform(OrientedBoundingBox& outBV, const DirectX::SimpleMath::Matrix& tranformMatrix);
		bool Intersect(const OrientedBoundingBox& other);

	private:
		std::vector<DirectX::SimpleMath::Vector3> m_vertices;
	};
}