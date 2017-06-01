#include "pch.h"
#include "AxisAlignedBoundingBox.h"

using namespace DirectX::SimpleMath;
using namespace CollisionDetection;

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{
}


AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

void AxisAlignedBoundingBox::CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points)
{
	m_originalPoints = points;
	m_min = m_max = points[0];

	for(auto& point : points)
	{
		m_min = Vector3::Min(m_min, point);
		m_max = Vector3::Max(m_max, point);
	}

	m_center = Vector3::Lerp(m_min, m_max, 0.5f);
	m_extents = (m_max - m_min) * 0.5f;

}

void AxisAlignedBoundingBox::Transform(AxisAlignedBoundingBox& outBV, const DirectX::SimpleMath::Matrix& transformMatrix)
{
	std::vector<Vector3> transformedPoints;
	transformedPoints.resize(m_originalPoints.size());
	Vector3::Transform(&m_originalPoints[0], m_originalPoints.size(), transformMatrix, &transformedPoints[0]);
	outBV.CreateFromPoints(transformedPoints);
}

bool AxisAlignedBoundingBox::Intersect(const AxisAlignedBoundingBox& other) const
{
	const float* const a_min[3]{ &m_min.x, &m_min.y, &m_min.z };
	const float* const a_max[3]{ &m_max.x, &m_max.y, &m_max.z };

	const float* const b_min[3]{ &other.m_min.x, &other.m_min.y, &other.m_min.z };
	const float* const b_max[3]{ &other.m_max.x, &other.m_max.y, &other.m_max.z };

	// Exit with no intersection if separated along an axis
	for(size_t i = 0; i < 3; ++i)
	{
		if (*a_max[i] < *b_min[i] || *a_min[i] > *b_max[i]) 
			return false;
	}

	// Overlapping on all axes means AABBs are intersecting
	return true;
}

const DirectX::SimpleMath::Vector3& AxisAlignedBoundingBox::GetCenter() const
{
	return m_center;
}

const DirectX::SimpleMath::Vector3& AxisAlignedBoundingBox::GetExtents() const
{
	return m_extents;
}
