#include "pch.h"
#include "ICollidable2D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

uint64_t ICollidable2D::s_ids = -1;

auto ICollidable2D::GetID() const -> const uint64_t&
{
	return m_id;
}

void ICollidable2D::initializeCollider()
{
	m_id = ++s_ids;
	createBoundingSphere();
	createAABB();
	createOBB();
}

void ICollidable2D::updateBoundingBoxesTransforms(const DirectX::SimpleMath::Matrix& worldMatrix)
{
	m_boundingSphere.Original.Transform(m_boundingSphere.Transformed, worldMatrix);
}

void ICollidable2D::createBoundingSphere()
{
	auto& vertices2d = GetVertices();
	std::vector<Vector3> vertices3d;
	vertices3d.reserve(vertices2d.size());
	for (auto& vertex : vertices2d)
		vertices3d.push_back(Vector3(vertex.x, vertex.y, 0.f));
	DirectX::BoundingSphere::CreateFromPoints(m_boundingSphere.Original, vertices3d.size(), &vertices3d[0], sizeof(Vector3));
}
