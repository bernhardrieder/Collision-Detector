#include "pch.h"
#include "CollisionDetector2D.h"


CollisionDetector2D::CollisionDetector2D()
{
}


CollisionDetector2D::~CollisionDetector2D()
{
}

bool CollisionDetector2D::isCollisionBoundingVolumeTest(const DirectX::BoundingSphere& lhs, const DirectX::BoundingSphere& rhs)
{
	return lhs.Intersects(rhs);
}
