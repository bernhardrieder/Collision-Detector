#pragma once
class Collideable2D
{
	static uint64_t s_ids;
public:
	virtual ~Collideable2D() = default;

	auto GetID() const -> const uint64_t&;
	const DirectX::BoundingSphere& GetBoundingSphereTransformed() const { return m_boundingSphere.Transformed; }
	const DirectX::BoundingBox& GetAxisAlignedBoundingBoxTransformed() const { return m_axisAlignedBoundingBox.Transformed; }
	const DirectX::BoundingOrientedBox& GetOrientedBoundingBoxTransformed() const { return m_orientedBoundingBox.Transformed; }

	virtual const std::vector<DirectX::SimpleMath::Vector2>& GetVertices() const = 0;

protected:
	void initializeCollider();
	void updateBoundingBoxesTransforms(const DirectX::SimpleMath::Matrix& scaleMatrix, const DirectX::SimpleMath::Matrix& rotationMatrix, const DirectX::SimpleMath::Matrix& translationMatrix);

private:
	void extract3dVerticesPositionsFromVertices();
	void createBoundingSphere();
	void createAABB();
	void createOBB();

	//https://msdn.microsoft.com/en-us/library/bb313876.aspx
	struct BoundingSphere
	{
		DirectX::BoundingSphere Original;
		DirectX::BoundingSphere Transformed;		
	} m_boundingSphere;

	struct AxisAlignedBoundingBox
	{
		DirectX::BoundingBox Original;
		DirectX::BoundingBox Transformed;
	} m_axisAlignedBoundingBox;

	struct OrientedBoundingBox
	{
		DirectX::BoundingOrientedBox Original;
		DirectX::BoundingOrientedBox Transformed;
	} m_orientedBoundingBox;

	std::vector<DirectX::SimpleMath::Vector3> m_verticesPositions3D;

	uint64_t m_id = -1;

	struct AppliedTransformMatrices
	{
		DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix Rotation = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix Translation = DirectX::SimpleMath::Matrix::Identity;
	} m_lastAppliedMatrices;
};

