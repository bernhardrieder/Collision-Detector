#include "pch.h"
#include "BoundingSphere.h"

using namespace DirectX::SimpleMath;
using namespace CollisionDetection;

BoundingSphere::BoundingSphere() : BoundingSphere(DirectX::SimpleMath::Vector3::Zero, 0.f)
{
}

BoundingSphere::BoundingSphere(const DirectX::SimpleMath::Vector3& vector3, float radius) : m_center(vector3), m_radius(radius)
{
}

BoundingSphere::~BoundingSphere()
{
}

auto BoundingSphere::GetCenter() const -> const DirectX::SimpleMath::Vector3&
{ return m_center; }

auto BoundingSphere::GetRadius() const -> const float&
{
	return m_radius;
}

void BoundingSphere::CreateFromPoints(const std::vector<DirectX::SimpleMath::Vector3>& points)
{
	Vector3 minX, maxX, minY, maxY, minZ, maxZ;
	GetMinAndMaxVertices(points, minX, maxX, minY, maxY, minZ, maxZ);

	// form the initial sphere
	Vector3 deltaX = maxX - minX;
	float distX = deltaX.Length();

	Vector3 deltaY = maxY - minY;
	float distY = deltaY.Length();

	Vector3 deltaZ = maxZ - minZ;
	float distZ = deltaZ.Length();

	Vector3 center;
	float radius;

	if (distX > distY)
	{
		if (distX > distZ)
		{
			center = Vector3::Lerp(maxX, minX, 0.5f);
			radius = distX * 0.5f;
		}
		else
		{
			center = Vector3::Lerp(maxZ, minZ, 0.5f);
			radius = distZ * 0.5f;
		}
	}
	else // Y >= X
	{
		if (distY > distZ)
		{
			center = Vector3::Lerp(maxY, minY, 0.5f);
			radius = distY * 0.5f;
		}
		else
		{
			center = Vector3::Lerp(maxZ, minZ, 0.5f);
			radius = distZ * 0.5f;
		}
	}

	// add any points not inside the sphere!!!
	for (size_t i = 0; i < points.size(); ++i)
	{
		Vector3 Delta = points[i] - center;

		float Dist = Delta.Length();

		if (Dist > radius)
		{
			radius = (radius + Dist) * 0.5f;
			center += Delta * (1.0f - radius / Dist);
		}
	}

	m_center = center;
	m_radius = radius;
}

void BoundingSphere::Transform(BoundingSphere& outBV, const DirectX::SimpleMath::Matrix& scaleRotationTranslationMatrix, const DirectX::SimpleMath::Matrix& scaleMatrix) const
{
	outBV.m_center = Vector3::Transform(m_center, scaleRotationTranslationMatrix);
	outBV.m_radius = m_radius * scaleMatrix._11;
}

bool BoundingSphere::Intersect(const BoundingSphere& other) const
{
	// see slide 12
	// |M1-M2| < r1+r2
	// |M1-M2|^2 < (r1+r2)^2
	// |M1-M2|^2 - (r1+r2)^2 < 0

	float distSquared = (other.m_center - m_center).LengthSquared();
	float radiusSquared = std::pow(other.m_radius + m_radius, 2);

	return distSquared - radiusSquared < 0;
}