#pragma once
namespace CollisionDetection
{
	class BoundingSphere
	{
	public:
		BoundingSphere();
		BoundingSphere(const DirectX::SimpleMath::Vector3& vector3, float radius);
		~BoundingSphere();

		void CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points);
		void Transform(BoundingSphere& outBV, const DirectX::SimpleMath::Matrix& scaleRotationTranslationMatrix, const DirectX::SimpleMath::Matrix& scaleMatrix) const;
		bool Intersect(const BoundingSphere& other) const;

		auto GetCenter() const -> const DirectX::SimpleMath::Vector3&;
		auto GetRadius() const -> const float&;

	private:
		DirectX::SimpleMath::Vector3 m_center;
		float m_radius;
	};
}