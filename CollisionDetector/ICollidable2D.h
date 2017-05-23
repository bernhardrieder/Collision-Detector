#pragma once
class ICollidable2D
{
	static uint64_t s_ids;
public:
	virtual ~ICollidable2D() = default;

	auto GetID() const -> const uint64_t&;
	const DirectX::BoundingSphere& GetBoundingSphereTransformed() const { return m_boundingSphere.Transformed; }
	void* const GetAABB() {}
	void* const GetOBB() {}

	virtual const std::vector<DirectX::SimpleMath::Vector2>& GetVertices() const = 0;

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

	uint64_t m_id = -1;
};

