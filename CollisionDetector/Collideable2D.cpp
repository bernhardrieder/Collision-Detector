#include "pch.h"
#include "Collideable2D.h"

using namespace DirectX::SimpleMath;
using namespace CollisionDetection;

uint64_t Collideable2D::s_ids = -1;

auto Collideable2D::GetID() const -> const uint64_t&
{
	return m_id;
}

const BoundingSphere& Collideable2D::GetBoundingSphereTransformed()
{
	m_boundingSphere.Original.Transform(m_boundingSphere.Transformed, m_lastAppliedMatrices.ScaleRotationTranslation, m_lastAppliedMatrices.Scale);
	return m_boundingSphere.Transformed;
}

const AxisAlignedBoundingBox& Collideable2D::GetAxisAlignedBoundingBoxTransformed()
{
	m_axisAlignedBoundingBox.Original.Transform(m_axisAlignedBoundingBox.Transformed, m_lastAppliedMatrices.ScaleRotationTranslation);
	return m_axisAlignedBoundingBox.Transformed;
}

const OrientedBoundingBox& Collideable2D::GetOrientedBoundingBoxTransformed()
{
	m_orientedBoundingBox.Original.Transform(m_orientedBoundingBox.Transformed, m_lastAppliedMatrices.Translation, m_lastAppliedMatrices.Scale, m_lastAppliedMatrices.Rotation);
	return m_orientedBoundingBox.Transformed;
}

auto Collideable2D::GetVertices3DTransformed() -> const std::vector<DirectX::SimpleMath::Vector3>&
{
	transformOriginalVertices3D(m_lastAppliedMatrices.ScaleRotationTranslation);
	return m_verticesPositions3D.Transformed;
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
	m_lastAppliedMatrices.Scale = scaleMatrix;
	m_lastAppliedMatrices.Rotation = rotationMatrix;
	m_lastAppliedMatrices.Translation = translationMatrix;

	m_lastAppliedMatrices.ScaleTranslation = scaleMatrix * Matrix::Identity * translationMatrix;
	m_lastAppliedMatrices.ScaleRotationTranslation = scaleMatrix * rotationMatrix * translationMatrix;
}

void Collideable2D::extract3dVerticesPositionsFromVertices()
{
	auto& vertices2d = GetVertices();
	m_verticesPositions3D.Original.reserve(vertices2d.size());
	m_verticesPositions3D.Transformed.reserve(vertices2d.size());
	for (auto& vertex : vertices2d)
	{
		m_verticesPositions3D.Original.push_back(Vector3(vertex.x, vertex.y, 0.f));
		m_verticesPositions3D.Transformed.push_back(Vector3(vertex.x, vertex.y, 0.f));
	}
}

void Collideable2D::transformOriginalVertices3D(const DirectX::SimpleMath::Matrix& transformMatrix)
{
	for (size_t i = 0; i < m_verticesPositions3D.Original.size(); ++i)
	{
		m_verticesPositions3D.Transformed[i] = Vector3::Transform(m_verticesPositions3D.Original[i], transformMatrix);
	}
}

void Collideable2D::createBoundingSphere()
{
	m_boundingSphere.Original.CreateFromPoints(m_verticesPositions3D.Original);
}

void Collideable2D::createAABB()
{
	m_axisAlignedBoundingBox.Original.CreateFromPoints(m_verticesPositions3D.Original);
}

void Collideable2D::createOBB()
{
	m_orientedBoundingBox.Original.CreateFromPoints(m_verticesPositions3D.Original);
}
