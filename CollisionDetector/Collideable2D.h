#pragma once
class Collideable2D
{
	static uint64_t s_ids;
public:
	virtual ~Collideable2D() = default;

	auto GetID() const -> const uint64_t&;
	const DirectX::BoundingSphere& GetBoundingSphereTransformed() const { return m_boundingSphere.Transformed; }
	void* GetAABB() {}
	void* GetOBB() {}

	virtual const std::vector<DirectX::SimpleMath::Vector2>& GetVertices() const = 0;
	auto GetLastAppliedWorldMatrix() const -> const DirectX::SimpleMath::Matrix&{ return m_lastAppliedWorldMatrix; }

protected:
	void initializeCollider();
	void updateBoundingBoxesTransforms(const DirectX::SimpleMath::Matrix& worldMatrix);

private:
	void createBoundingSphere();
	void createAABB();
	void createOBB();

	struct BoundingSphere
	{
		DirectX::BoundingSphere Original;
		DirectX::BoundingSphere Transformed;		
	} m_boundingSphere;

	DirectX::SimpleMath::Matrix m_lastAppliedWorldMatrix = DirectX::SimpleMath::Matrix::Identity;

	uint64_t m_id = -1;
};

