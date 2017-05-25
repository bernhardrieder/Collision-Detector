#include "pch.h"
#include "Collideable2D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

uint64_t Collideable2D::s_ids = -1;

auto Collideable2D::GetID() const -> const uint64_t&
{
	return m_id;
}

const DirectX::BoundingSphere& Collideable2D::GetBoundingSphereTransformed()
{
	m_boundingSphere.Original.Transform(m_boundingSphere.Transformed, m_lastAppliedMatrices.ScaleRotationTranslation);
	return m_boundingSphere.Transformed;
}

const DirectX::BoundingBox& Collideable2D::GetAxisAlignedBoundingBoxTransformed()
{
	transformOriginalVertices3D(m_lastAppliedMatrices.Rotation);
	createAABB();
	m_axisAlignedBoundingBox.Original.Transform(m_axisAlignedBoundingBox.Transformed, m_lastAppliedMatrices.ScaleTranslation);
	return m_axisAlignedBoundingBox.Transformed;
}

const DirectX::BoundingOrientedBox& Collideable2D::GetOrientedBoundingBoxTransformed()
{
	m_orientedBoundingBox.Original.Transform(m_orientedBoundingBox.Transformed, m_lastAppliedMatrices.ScaleRotationTranslation);
	return m_orientedBoundingBox.Transformed;
}

auto Collideable2D::GetVertices3DRotated() -> const std::vector<DirectX::SimpleMath::Vector3>&
{
	transformOriginalVertices3D(m_lastAppliedMatrices.Rotation);
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
	DirectX::BoundingSphere::CreateFromPoints(m_boundingSphere.Original, m_verticesPositions3D.Original.size(), &m_verticesPositions3D.Original[0], sizeof(Vector3));
}

void Collideable2D::createAABB()
{
	DirectX::BoundingBox::CreateFromPoints(m_axisAlignedBoundingBox.Original, m_verticesPositions3D.Transformed.size(), &m_verticesPositions3D.Transformed[0], sizeof(Vector3));
}

void Collideable2D::createOBB()
{
	//todo: improve create from points?! -> this results in wrong OBBs!
	DirectX::BoundingOrientedBox::CreateFromPoints(m_orientedBoundingBox.Original, m_verticesPositions3D.Original.size(), &m_verticesPositions3D.Original[0], sizeof(Vector3));
}
