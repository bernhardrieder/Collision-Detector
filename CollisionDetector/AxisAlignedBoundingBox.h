#pragma once
namespace CollisionDetection
{
	class AxisAlignedBoundingBox
	{
	public:
		AxisAlignedBoundingBox();
		~AxisAlignedBoundingBox();

		void CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points);
		void Transform(AxisAlignedBoundingBox& outBV, const DirectX::SimpleMath::Matrix& tranformMatrix);
		bool Intersect(const AxisAlignedBoundingBox& other);

	private:
		std::vector<DirectX::SimpleMath::Vector3> m_vertices;
	};
}