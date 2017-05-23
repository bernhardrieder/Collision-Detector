#include "pch.h"
#include "Collideable2D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

uint64_t Collideable2D::s_ids = -1;

auto Collideable2D::GetID() const -> const uint64_t&
{
	return m_id;
}

void Collideable2D::initializeCollider()
{
	m_id = ++s_ids;
	extract3dVerticesPositionsFromVertices();
	createBoundingSphere();
	createAABB();
	createOBB();
}

void Collideable2D::updateBoundingBoxesTransforms(const DirectX::SimpleMath::Matrix& scaleMatrix, const DirectX::SimpleMath::Matrix& rotationMatrix, const DirectX::SimpleMath::Matrix& translationMatrix)
{
	if(rotationMatrix != m_lastAppliedMatrices.Rotation)
		createAABB();
	
	m_lastAppliedMatrices.Scale = scaleMatrix;
	m_lastAppliedMatrices.Rotation = rotationMatrix;
	m_lastAppliedMatrices.Translation = translationMatrix;

	Matrix scaleTranslation = scaleMatrix*Matrix::Identity*translationMatrix;
	Matrix scaleRotationTranslation = scaleMatrix*rotationMatrix*translationMatrix;
	m_boundingSphere.Original.Transform(m_boundingSphere.Transformed, scaleRotationTranslation);
	m_axisAlignedBoundingBox.Original.Transform(m_axisAlignedBoundingBox.Transformed, scaleTranslation);
	m_orientedBoundingBox.Original.Transform(m_orientedBoundingBox.Transformed, scaleRotationTranslation);
}

void Collideable2D::extract3dVerticesPositionsFromVertices()
{
	auto& vertices2d = GetVertices();
	m_verticesPositions3D.reserve(vertices2d.size());
	for (auto& vertex : vertices2d)
		m_verticesPositions3D.push_back(Vector3(vertex.x, vertex.y, 0.f));
}

void Collideable2D::createBoundingSphere()
{
	DirectX::BoundingSphere::CreateFromPoints(m_boundingSphere.Original, m_verticesPositions3D.size(), &m_verticesPositions3D[0], sizeof(Vector3));
}

void Collideable2D::createAABB()
{
	DirectX::BoundingBox::CreateFromPoints(m_axisAlignedBoundingBox.Original, m_verticesPositions3D.size(), &m_verticesPositions3D[0], sizeof(Vector3));
}

void Collideable2D::createOBB()
{
	DirectX::BoundingOrientedBox::CreateFromPoints(m_orientedBoundingBox.Original, m_verticesPositions3D.size(), &m_verticesPositions3D[0], sizeof(Vector3));
}
