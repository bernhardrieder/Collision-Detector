#pragma once
namespace CollisionDetection
{
	class OrientedBoundingBox
	{
	public:
		OrientedBoundingBox();
		~OrientedBoundingBox();

		void CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points);
		void Transform(OrientedBoundingBox& outBV, const DirectX::SimpleMath::Matrix& translationMatrix, const DirectX::SimpleMath::Matrix& scaleMatrix, const DirectX::SimpleMath::Matrix& rotationMatrix);
		bool Intersect(const OrientedBoundingBox& other) const;

		const DirectX::SimpleMath::Vector3& GetCenter() const;
		const DirectX::SimpleMath::Vector3& GetExtents() const;

	private:
		DirectX::SimpleMath::Vector3 m_center;
		DirectX::SimpleMath::Vector3 m_extents;
		DirectX::SimpleMath::Vector3 m_up;
		DirectX::SimpleMath::Vector3 m_forward;
		DirectX::SimpleMath::Vector3 m_right;
		DirectX::SimpleMath::Vector3* m_localAxes[3];
	};
}