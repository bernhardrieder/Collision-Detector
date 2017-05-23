#include "pch.h"
#include "CollisionDetector2D.h"


CollisionDetector2D::CollisionDetector2D()
{
}


CollisionDetector2D::~CollisionDetector2D()
{
}

void CollisionDetector2D::RegisterCollidable(const Collideable2D* const collidable)
{
	m_collidables.push_back(CollisionObject(collidable));
}

void CollisionDetector2D::DeregisterCollidable(const Collideable2D* const collidable)
{
	//todo
}

void CollisionDetector2D::DetectAndUpdateCollisionsOnAllRegisteredObjects()
{
	for(auto& owner : m_collidables)
	{
		for(const auto& other : m_collidables)
		{
			if (owner.Object->GetID() == other.Object->GetID())
				continue;

			/************************* Bounding Volume *************************/
			if (!isCollisionDetectedWithBoundingVolumeTest(owner.Object->GetBoundingSphereTransformed(), other.Object->GetBoundingSphereTransformed()))
			continue;

			owner.Collisions.push_back(Collision2D(other.Object));
			Collision2D& collision = owner.Collisions[owner.Collisions.size()-1];
			collision.LastDetectedType = BoundingVolume;

			/************************* AABB *************************/
			if (!isCollisionDetectedWithAABB())
				continue;
			collision.LastDetectedType = AABB;

			/************************* OBB *************************/
			if (!isCollisionDetectedWithOBB())
				continue;
			collision.LastDetectedType = OBB;

			/************************* Minkovsky Sum *************************/
			if (!isCollisionDetectedWithMinkovskySum())
				continue;
			collision.LastDetectedType = MinkovskySum;
		}
	}
}


bool CollisionDetector2D::isCollisionDetectedWithBoundingVolumeTest(const DirectX::BoundingSphere& lhs, const DirectX::BoundingSphere& rhs)
{
	return lhs.Intersects(rhs);
}

bool CollisionDetector2D::isCollisionDetectedWithAABB()
{
	return false;
}

bool CollisionDetector2D::isCollisionDetectedWithOBB()
{
	return false;
}

bool CollisionDetector2D::isCollisionDetectedWithMinkovskySum()
{
	return false;
}
