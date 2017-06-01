#pragma once
namespace CollisionDetection
{
	class AxisAlignedBoundingBox
	{
	public:
		AxisAlignedBoundingBox();
		~AxisAlignedBoundingBox();

		void CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points);
		void Transform(AxisAlignedBoundingBox& outBV, const DirectX::SimpleMath::Matrix& transformMatrix);
		bool Intersect(const AxisAlignedBoundingBox& other) const;

		const DirectX::SimpleMath::Vector3& GetCenter() const;
		const DirectX::SimpleMath::Vector3& GetExtents() const;

	private:
		DirectX::SimpleMath::Vector3 m_center = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_extents = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_min = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_max = DirectX::SimpleMath::Vector3::Zero;

		std::vector<DirectX::SimpleMath::Vector3> m_originalPoints;
	};
}