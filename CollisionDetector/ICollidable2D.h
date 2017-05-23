#pragma once
class ICollidable2D
{
	static uint64_t s_ids;
public:

	uint64_t GetID() const;
	const DirectX::BoundingSphere& GetBoundingSphere() const { return m_boundingSphere; }
	void* const GetAABB() {}
	void* const GetOBB() {}

	virtual std::vector<DirectX::SimpleMath::Vector2>& GetVertices() const = 0;

protected:
	void initialize();
	virtual void updateTransforms() = 0;

	void createBoundingSphere();
	void createAABB();
	void createOBB();

	DirectX::BoundingSphere m_boundingSphere;

private:
	uint64_t m_id = -1;
};

