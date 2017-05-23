#pragma once
class ICollidable2D
{
public:
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	void* const GetAABB() {}
	void* const GetOBB() {}

	virtual std::vector<DirectX::SimpleMath::Vector2>& GetVertices() const = 0;

protected:
	void createBoundingSphere();

	DirectX::BoundingSphere m_boundingSphere;

};

