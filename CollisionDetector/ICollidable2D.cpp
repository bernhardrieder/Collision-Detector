#include "pch.h"
#include "ICollidable2D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void ICollidable2D::createBoundingSphere()
{
	auto& vertices2d = GetVertices();
	std::vector<Vector3> vertices3d;
	vertices3d.reserve(vertices2d.size());
	for (auto& vertex : vertices2d)
		vertices3d.push_back(Vector3(vertex.x, vertex.y, 0.f));
	BoundingSphere::CreateFromPoints(m_boundingSphere, vertices3d.size(), &vertices3d[0], sizeof(Vector3));
}
